import sqlite3
import io
import webbrowser
import os
import numpy as np
import pandas as pd
from sklearn.manifold import TSNE
from sentence_transformers import SentenceTransformer
from pyvis.network import Network
from sklearn.metrics.pairwise import cosine_similarity

# Load data from SQLite database
conn = sqlite3.connect('images.db')
df = pd.read_sql_query("SELECT id, title, hashtags, image FROM Images", conn)
conn.close()

# Concatenate title and hashtags
df['text'] = df['title'] + ' ' + df['hashtags']

# Generate embeddings
model = SentenceTransformer('all-MiniLM-L6-v2')
embeddings = model.encode(df['text'].tolist())

perplexity_value = min(30, len(df) - 1)  # Ensure perplexity is less than the number of samples

tsne = TSNE(n_components=2, perplexity=perplexity_value, random_state=42)
coords = tsne.fit_transform(embeddings)

# Create a network graph
net = Network(
    height='750px',
    width='100%',
    bgcolor='#222222',
    font_color='white',
    notebook=False  # Set notebook parameter to False
)
net.barnes_hut()

# Ensure 'temp_images' directory exists
if not os.path.exists('temp_images'):
    os.makedirs('temp_images')

# Add nodes to the graph
for idx, (x, y) in enumerate(coords):
    node_id = int(df.iloc[idx]['id'])  # Ensure node_id is int
    title = df.iloc[idx]['title']
    image_data = df.iloc[idx]['image']

    # Save image to disk
    image_path = f'temp_images/{node_id}.jpg'
    with open(image_path, 'wb') as f:
        f.write(image_data)

    net.add_node(
        node_id,
        label=title,
        x=float(x),
        y=float(y),
        title=title,
        physics=True,
        image=image_path,
        shape='image'
    )

# Add edges based on proximity
similarity_matrix = cosine_similarity(embeddings)
threshold = 0.7  # Adjust based on desired connectivity

for i in range(len(df)):
    for j in range(i + 1, len(df)):
        similarity = similarity_matrix[i][j]
        if similarity > threshold:
            source_id = int(df.iloc[i]['id'])  # Ensure IDs are int
            target_id = int(df.iloc[j]['id'])
            net.add_edge(
                source_id,
                target_id,
                value=float(similarity),
                width=2 * (similarity - threshold),
                color='#AAAAAA'
            )

# Optimize physics settings for aesthetics
net.toggle_physics(True)
net.force_atlas_2based(gravity=-50)

# Display the graph
net.show_buttons(filter_=['physics'])
net.show('graph.html', notebook=False)  # Set notebook parameter to False
webbrowser.open('graph.html')

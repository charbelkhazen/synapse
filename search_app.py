import sqlite3
import tkinter as tk
from tkinter import ttk
from PIL import Image
import io
import subprocess
import os

# Connect to the SQLite3 database
conn = sqlite3.connect('images.db')

def search_images(event=None):
    keyword = search_entry.get()
    cursor = conn.cursor()
    cursor.execute("""
        SELECT id, title, hashtags FROM Images 
        WHERE title LIKE ? OR hashtags LIKE ?
    """, ('%' + keyword + '%', '%' + keyword + '%'))
    results = cursor.fetchall()

    # Clear the Treeview and display search results
    tree.delete(*tree.get_children())
    for result in results:
        tree.insert('', tk.END, values=(result[1], result[2]))

    global images_dict
    images_dict = {result[1]: result[0] for result in results}

def update_suggestions(event):
    keyword = search_entry.get()
    if not keyword:
        suggestion_tree.pack_forget()
        return

    cursor = conn.cursor()
    cursor.execute("""
        SELECT id, title, hashtags FROM Images 
        WHERE title LIKE ? OR hashtags LIKE ?
    """, ('%' + keyword + '%', '%' + keyword + '%'))
    suggestions = cursor.fetchall()

    suggestion_tree.delete(*suggestion_tree.get_children())
    for suggestion in suggestions:
        suggestion_tree.insert('', tk.END, values=(suggestion[1], suggestion[2]))
    
    if suggestions:
        suggestion_tree.pack(padx=5, pady=2)
    else:
        suggestion_tree.pack_forget()

def select_suggestion(event):
    selected_item = suggestion_tree.focus()
    if selected_item:
        selected_values = suggestion_tree.item(selected_item, 'values')
        selected_title = selected_values[0]
        search_entry.delete(0, tk.END)
        search_entry.insert(0, selected_title)
        suggestion_tree.pack_forget()
        search_images()

def display_image(title):
    image_id = images_dict[title]
    cursor = conn.cursor()
    cursor.execute("SELECT image FROM Images WHERE id=?", (image_id,))
    image_data = cursor.fetchone()[0]

    if image_data:
        img = Image.open(io.BytesIO(image_data))
        temp_file = "temp_image.png"
        img.save(temp_file)
        subprocess.run(["xdg-open", temp_file] if os.name != 'nt' else ["start", temp_file], shell=True)

def on_treeview_double_click(event):
    selected_item = tree.focus()
    if selected_item:
        selected_values = tree.item(selected_item, 'values')
        selected_title = selected_values[0]
        display_image(selected_title)

# Set up the GUI
root = tk.Tk()
root.title("Terminal-like Image Search")
root.geometry("700x600")
root.configure(bg="#000000")

# Style Configuration
style = ttk.Style()
style.theme_use("default")

# Configure Treeview style
style.configure("Custom.Treeview",
                background="#000000",
                foreground="#00FF00",
                fieldbackground="#000000",
                rowheight=25)

style.map("Custom.Treeview",
          background=[('selected', '#555555')],
          foreground=[('selected', '#FFFFFF')])

# Configure headings
style.configure("Custom.Treeview.Heading",
                background="#000000",
                foreground="#00FF00",
                font=("Courier", 12, "bold"))

# Header Label
header_label = tk.Label(root, text="Terminal-like Image Search", font=("Courier", 18, "bold"), fg="#00FF00", bg="#000000")
header_label.pack(pady=10)

# Search bar with a monospaced font
search_entry = tk.Entry(root, width=50, font=("Courier", 14), bg="#1e1e1e", fg="#00FF00", insertbackground='white', relief="flat")
search_entry.pack(pady=10)
search_entry.bind("<KeyRelease>", update_suggestions)
search_entry.bind("<Return>", search_images)

# Dynamic suggestion Treeview
suggestion_tree = ttk.Treeview(root, columns=("Title", "Hashtags"), show='headings', height=5, style="Custom.Treeview")
suggestion_tree.heading("Title", text="Title")
suggestion_tree.heading("Hashtags", text="Hashtags")
suggestion_tree.column("Title", width=250, anchor='w')
suggestion_tree.column("Hashtags", width=400, anchor='w')
suggestion_tree.bind("<Button-1>", select_suggestion)

# Frame for Treeview with scrollbar
frame = tk.Frame(root, bg="#000000")
frame.pack(pady=10, fill=tk.BOTH, expand=True)

# Scrollbar for Treeview
scrollbar = tk.Scrollbar(frame, orient=tk.VERTICAL)

# Main Treeview to display titles and hashtags
tree = ttk.Treeview(frame, columns=("Title", "Hashtags"), show='headings', selectmode='browse', yscrollcommand=scrollbar.set, style="Custom.Treeview")
tree.heading("Title", text="Title")
tree.heading("Hashtags", text="Hashtags")
tree.column("Title", width=250, anchor='w')
tree.column("Hashtags", width=400, anchor='w')

tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5, pady=5)
scrollbar.config(command=tree.yview)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

# Bind the Treeview selection to double-click event
tree.bind('<Double-1>', on_treeview_double_click)

root.mainloop()

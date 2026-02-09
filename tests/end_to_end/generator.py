from pathlib import Path
import random
import tkinter as tk
from tkinter import ttk

# ---------- настройки дизайна ----------
BG = "#F6F7FB"        # фон окна
CARD = "#FFFFFF"      # фон карточек
TEXT = "#111827"      # основной текст
MUTED = "#6B7280"     # вторичный текст

PRIMARY = "#3B82F6"   # generate
DANGER = "#F43F5E"    # close
ACCENT = "#10B981"    # плюс
MINUS = "#EF4444"     # минус

FONT_TITLE = ("Segoe UI", 18, "bold")
FONT_LABEL = ("Segoe UI", 11)
FONT_ENTRY = ("Segoe UI", 16, "bold")
FONT_BTN = ("Segoe UI", 12, "bold")


# ---------- окно ----------
window = tk.Tk()
window.title("generator")
window.geometry("600x870")
window.resizable(False, False)
window.configure(bg=BG)

style = ttk.Style()
style.theme_use("clam")

style.configure("TFrame", background=BG)
style.configure("Card.TFrame", background=CARD)

style.configure("Title.TLabel", background=BG, foreground=TEXT, font=FONT_TITLE)
style.configure("Sub.TLabel", background=CARD, foreground=MUTED, font=FONT_LABEL)

style.configure("TEntry", font=FONT_ENTRY, padding=(10, 10))

style.configure("Primary.TButton", font=FONT_BTN, padding=(12, 12), background=PRIMARY, foreground="white")
style.map("Primary.TButton", background=[("active", "#2563EB")])

style.configure("Danger.TButton", font=FONT_BTN, padding=(12, 12), background=DANGER, foreground="white")
style.map("Danger.TButton", background=[("active", "#E11D48")])

def nice_btn(parent, text, bg, active_bg, command):
    return tk.Button(parent, text=text, font=("Segoe UI", 18, "bold"), bg=bg, fg="white",
                     activebackground=active_bg, activeforeground="white", relief="flat", bd=0,
                     cursor="hand2", command=command
    )


# ---------- логика считывания/изменения ----------
def safe_int(entry: tk.Entry) -> int:
    try:
        return int(entry.get())
    except ValueError:
        return 0

def set_int(entry: tk.Entry, value: int):
    entry.delete(0, tk.END)
    entry.insert(0, str(value))

def change(entry: tk.Entry, delta: int, min_value: int = 0):
    value = safe_int(entry) + delta
    if value < min_value:
        value = min_value
    set_int(entry, value)

def close_window():
    window.destroy()

def generate_tests():
    files_count = safe_int(entry_files)
    tests_per_file = safe_int(entry_tests)

    if files_count <= 0:
        print("Error: the number of files must be > 0")
        return

    if tests_per_file <= 0:
        print("Error: the number of numbers per file must be > 0")
        return

    range_from = safe_int(entry_range_from)
    range_to = safe_int(entry_range_to)
    
    if range_from > range_to:
        print("Error: Range From cannot be greater than Range To")
        return

    rel = entry_path.get().strip() or "./"
    base_dir = Path(__file__).resolve().parent
    out_dir = (base_dir / rel).resolve()

    data_dir = out_dir / "data"
    answ_dir = out_dir / "answ"
    data_dir.mkdir(parents=True, exist_ok=True)
    answ_dir.mkdir(parents=True, exist_ok=True)

    for i in range(1, files_count + 1):
        nums = [random.randint(range_from, range_to) for _ in range(tests_per_file)]

        data_line = str(tests_per_file) + " " + " ".join(map(str, nums)) + "\n"
        (data_dir / f"{i}.txt").write_text(data_line, encoding="utf-8")

        nums_sorted = sorted(nums)
        answ_line = " ".join(map(str, nums_sorted)) + "\n"
        (answ_dir / f"{i}.txt").write_text(answ_line, encoding="utf-8")

    print(f"Done: \n{files_count} files created in \n{data_dir}, \nand answers in \n{answ_dir}")

    close_window()


# ---------- общий контейнер ----------
root = ttk.Frame(window)
root.pack(fill="both", expand=True, padx=18, pady=18)

title = ttk.Label(root, text="Test Generator", style="Title.TLabel")
title.pack(anchor="w", pady=(0, 12))


# ---------- карточка: number of files ----------
card1 = ttk.Frame(root, style="Card.TFrame")
card1.pack(fill="x", pady=10)

card1.grid_columnconfigure(1, weight=1)

ttk.Label(card1, text="Number of files", style="Sub.TLabel").grid(row=0, column=0, columnspan=3, sticky="w", padx=16, pady=(14, 8))

btn_files_minus = nice_btn(card1, "−", MINUS, "#DC2626", lambda: change(entry_files, -1))
btn_files_minus.grid(row=1, column=0, padx=(16, 10), pady=(0, 16), sticky="ew")

entry_files = ttk.Entry(card1, justify="center")
entry_files.grid(row=1, column=1, padx=0, pady=(0, 16), sticky="ew")
set_int(entry_files, 1)

btn_files_plus = nice_btn(card1, "+", ACCENT, "#059669", lambda: change(entry_files, +1))
btn_files_plus.grid(row=1, column=2, padx=(10, 16), pady=(0, 16), sticky="ew")

btn_files_minus.configure(height=1)
btn_files_plus.configure(height=1)

# ---------- карточка: range ----------
card3 = ttk.Frame(root, style="Card.TFrame")
card3.pack(fill="x", pady=10)

card3.grid_columnconfigure(0, weight=1)
card3.grid_columnconfigure(2, weight=1)

ttk.Label(card3, text="Range of numbers", style="Sub.TLabel").grid(
    row=0, column=0, columnspan=3, sticky="w", padx=16, pady=(14, 8)
)

entry_range_from = ttk.Entry(card3, justify="center")
entry_range_from.grid(row=1, column=0, padx=(16, 8), pady=(0, 16), sticky="ew")
set_int(entry_range_from, 0)

dash = ttk.Label(card3, text="—", style="Sub.TLabel") 
dash.grid(row=1, column=1, padx=0, pady=(0, 16))

entry_range_to = ttk.Entry(card3, justify="center")
entry_range_to.grid(row=1, column=2, padx=(8, 16), pady=(0, 16), sticky="ew")
set_int(entry_range_to, 10**9)


# ---------- карточка: tests per file ----------
card2 = ttk.Frame(root, style="Card.TFrame")
card2.pack(fill="x", pady=10)

card2.grid_columnconfigure(1, weight=1)

ttk.Label(card2, text="Numbers per file", style="Sub.TLabel").grid(row=0, column=0, columnspan=3, sticky="w", padx=16, pady=(14, 8))

btn_tests_minus = nice_btn(card2, "−", MINUS, "#DC2626", lambda: change(entry_tests, -1))
btn_tests_minus.grid(row=1, column=0, padx=(16, 10), pady=(0, 16), sticky="ew")

entry_tests = ttk.Entry(card2, justify="center")
entry_tests.grid(row=1, column=1, padx=0, pady=(0, 16), sticky="ew")
set_int(entry_tests, 1)

btn_tests_plus = nice_btn(card2, "+", ACCENT, "#059669", lambda: change(entry_tests, +1))
btn_tests_plus.grid(row=1, column=2, padx=(10, 16), pady=(0, 16), sticky="ew")

btn_tests_minus.configure(height=1)
btn_tests_plus.configure(height=1)

# ---------- карточка: path ----------
card4 = ttk.Frame(root, style="Card.TFrame")
card4.pack(fill="x", pady=10)
card4.grid_columnconfigure(1, weight=1)

ttk.Label(card4, text="Output folder", style="Sub.TLabel").grid(
    row=0, column=0, columnspan=2, sticky="w", padx=16, pady=(14, 8)
)

prefix = ttk.Label(card4, text="curr_dir/", style="Sub.TLabel")
prefix.grid(row=1, column=0, padx=(16, 8), pady=(0, 16), sticky="w")

entry_path = ttk.Entry(card4, justify="left")
entry_path.grid(row=1, column=1, padx=(0, 16), pady=(0, 16), sticky="ew")

entry_path.insert(0, "./")   



# ---------- нижние кнопки ----------
bottom = ttk.Frame(root)
bottom.pack(fill="x", pady=(18, 0))

bottom.grid_columnconfigure(0, weight=1)
bottom.grid_columnconfigure(1, weight=1)

btn_generate = ttk.Button(bottom, text="Generate tests", style="Primary.TButton", command=generate_tests)
btn_generate.grid(row=0, column=0, sticky="ew", padx=(0, 10))

btn_close = ttk.Button(bottom, text="Close", style="Danger.TButton", command=close_window)
btn_close.grid(row=0, column=1, sticky="ew", padx=(10, 0))

window.mainloop()

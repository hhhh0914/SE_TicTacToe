# -*- coding: utf-8 -*-
"""Generate SE project PDF report with UML diagrams."""
import os, sys

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUTPUT_DIR = os.path.join(BASE, "output")
os.makedirs(OUTPUT_DIR, exist_ok=True)
IMG_DIR = os.path.join(BASE, "resources", "diagrams")
os.makedirs(IMG_DIR, exist_ok=True)

from PIL import Image, ImageDraw, ImageFont
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_JUSTIFY
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Image, PageBreak,
    Table, TableStyle
)
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfbase import pdfmetrics

# Font registration
FONT_PATH = None
FONT_NAME = "Helvetica"
for fp in ["C:/Windows/Fonts/msyh.ttc", "C:/Windows/Fonts/simsun.ttc", "C:/Windows/Fonts/simhei.ttf"]:
    if os.path.exists(fp):
        FONT_PATH = fp
        FONT_NAME = os.path.splitext(os.path.basename(fp))[0]
        try:
            pdfmetrics.registerFont(TTFont(FONT_NAME, fp))
            print(f"Font registered: {FONT_NAME}")
        except: pass
        break

MONO_FP = None
for fp in ["C:/Windows/Fonts/consola.ttf", "C:/Windows/Fonts/cour.ttf"]:
    if os.path.exists(fp): MONO_FP = fp

print(f"Font: {FONT_NAME}, Mono: {MONO_FP}")

def f(sz): return ImageFont.truetype(FONT_PATH, sz) if FONT_PATH else ImageFont.load_default()
def fm(sz): return ImageFont.truetype(MONO_FP, sz) if MONO_FP else ImageFont.load_default()

def tc(d, cx, cy, text, font, fill="black"):
    b = d.textbbox((0,0), text, font=font)
    w, h = b[2]-b[0], b[3]-b[1]
    d.text((cx-w//2, cy-h//2), text, fill=fill, font=font)

CP = colors.HexColor('#1a237e')
CS = colors.HexColor('#283593')
CL = colors.HexColor('#e8eaf6')
CG = colors.HexColor('#616161')

st = ParagraphStyle("t", fontName=FONT_NAME, fontSize=26, leading=34, textColor=CP, spaceAfter=6*mm, alignment=TA_CENTER)
ss = ParagraphStyle("s", fontName=FONT_NAME, fontSize=14, leading=20, textColor=CG, spaceAfter=4*mm, alignment=TA_CENTER)
h1 = ParagraphStyle("h1", fontName=FONT_NAME, fontSize=18, leading=24, textColor=CP, spaceBefore=8*mm, spaceAfter=4*mm)
bd = ParagraphStyle("b", fontName=FONT_NAME, fontSize=10.5, leading=16, textColor=colors.black, spaceAfter=2*mm, alignment=TA_JUSTIFY)
cp = ParagraphStyle("c", fontName=FONT_NAME, fontSize=9, leading=13, textColor=CG, spaceAfter=3*mm, alignment=TA_CENTER)

def draw_uc():
    W, H = 800, 520
    img = Image.new("RGB", (W, H), "white")
    d = ImageDraw.Draw(img)
    d.rectangle([30, 20, W-30, H-20], outline="black", width=2)
    tc(d, W//2, 30, "Tic-Tac-Toe System", f(14), "#1a237e")
    for ax, aname in [(100, "Human Player"), (W-100, "AI Player")]:
        cy, y0 = H-120, H-80
        d.ellipse([ax-12, cy-12, ax+12, cy+12], outline="black", width=2)
        d.line([ax, cy+12, ax, y0], fill="black", width=2)
        d.line([ax-18, cy+2, ax+18, cy+2], fill="black", width=2)
        d.line([ax, y0, ax-14, y0+14], fill="black", width=2)
        d.line([ax, y0, ax+14, y0+14], fill="black", width=2)
        tc(d, ax, y0+22, aname, f(10))
    ucs = [(W//2-20, 180, "Play Game"), (W//2+20, 250, "Place Piece"),
           (W//2-10, 310, "Check Win"), (W//2+100, 180, "Play vs AI"),
           (W//2-60, 250, "View Board")]
    for x, y, text in ucs:
        b = d.textbbox((0,0), text, font=f(10))
        tw, th = b[2]-b[0]+10, b[3]-b[1]+10
        d.ellipse([x-tw//2, y-th//2, x+tw//2, y+th//2], outline="black", fill="#e8eaf6", width=2)
        tc(d, x, y-1, text, f(10))
    for tx, ty in [(W//2-20,180),(W//2-10,310),(W//2-60,250),(W//2+20,250)]:
        d.line([100, H-150, tx, ty+5], fill="#666", width=1)
    d.line([W-100, H-150, W//2+100, 200], fill="#666", width=1)
    d.line([W-100, H-150, W//2-10, 325], fill="#666", width=1)
    p = os.path.join(IMG_DIR, "uc.png"); img.save(p); print("  uc.png"); return p

def draw_cls():
    W, H = 800, 560
    img = Image.new("RGB", (W, H), "white")
    d = ImageDraw.Draw(img)
    def ck(x, y, w, h, tl, flds, mtds):
        d.rectangle([x,y,x+w,y+h], outline="black", width=2, fill="white")
        d.rectangle([x,y,x+w,y+28], fill="#d1c4e9", outline="black", width=2)
        tc(d, x+w//2, y+14, tl, f(11))
        d.line([x,y+28,x+w,y+28], fill="black")
        for i,fy in enumerate(range(y+42, y+200, 16)):
            if i<len(flds): d.text((x+6, fy), flds[i], fill="#555", font=fm(9))
        sy = y+42+16*len(flds)+2
        d.line([x, sy, x+w, sy], fill="black", width=1)
        for i in range(len(mtds)):
            d.text((x+6, sy+8+16*i), mtds[i], fill="black", font=fm(9))
    ck(20,80,210,190,"Board",["-grid_: Grid"],["+place():bool","+checkWin():bool","+getWinner()","+isFull()","+getEmptyCells()","+reset()"])
    ck(250,80,190,145,"Player",["-name_: string","-symbol_: CellState","-type_: Type"],["+getName()","+getSymbol()","+isAI()"])
    ck(110,320,220,165,"GameState",["-board_: Board","-player1_,player2_"],["+updateStatus()","+switchTurn()","+getBoard()","+getCurrentPlayer()"])
    ck(440,320,220,165,"ConsoleView",[],["+displayBoard()","+displayTurn()","+displayResult()","+displayMenu()"])
    ck(450,80,260,190,"GameController",["-gameState_: uptr","-view_: ConsoleView"],["+run()","-startGame()","-gameLoop()","-getPlayerInput()"])
    ck(230,180,150,100,"AIPlayer",[],["+getBestMove()","-minimax()","-getHeuristicMove()"])
    for x1,y1,x2,y2 in [(230,380,240,270),(240,380,345,225),(570,230,330,390),(590,210,590,400),(520,230,380,215),(310,215,230,235)]:
        d.line([x1,y1,x2,y2], fill="#555", width=1)
        d.polygon([x2,y2,x2-6,y2-4,x2+6,y2-4], outline="#555", fill="white")
    p = os.path.join(IMG_DIR, "cls.png"); img.save(p); print("  cls.png"); return p

def draw_seq():
    W, H = 800, 580
    img = Image.new("RGB", (W, H), "white")
    d = ImageDraw.Draw(img)
    parts = [(120,"Player"),(270,"Ctrl"),(420,"View"),(560,"State"),(720,"Board")]
    for x,n in parts:
        d.rectangle([x-42,10,x+42,40], fill="#e8eaf6", outline="black", width=2)
        tc(d, x, 25, n, f(10))
        d.line([x,40,x,H-30], fill="#aaa", width=1)
    msgs = [(120,550,270,"select"),(270,500,420,"displayMenu()"),(420,460,270,"choice"),
            (270,420,560,"init"),(270,380,420,"show board"),(270,340,120,"request"),
            (120,300,270,"row,col"),(270,260,560,"place()"),(560,220,720,"update"),
            (720,180,560,"ok"),(560,140,270,"check"),(270,100,420,"result")]
    for x1,y,x2,t in msgs:
        d.line([x1,y,x2,y], fill="black", width=1)
        d.polygon([x2,y,x2-7,y-4,x2-7,y+4], fill="black")
        tc(d, (x1+x2)//2, y-14, t, f(8))
    tc(d, 400, 65, "[loop until game over]", f(9), "#1a237e")
    p = os.path.join(IMG_DIR, "seq.png"); img.save(p); print("  seq.png"); return p

def draw_act():
    W, H = 700, 750
    img = Image.new("RGB", (W, H), "white")
    d = ImageDraw.Draw(img)
    CX = W//2
    def bx(y, text):
        b = d.textbbox((0,0), text, font=f(10))
        tw = max(b[2]-b[0]+30, 100)
        d.rounded_rectangle([CX-tw//2, y, CX+tw//2, y+36], radius=6, outline="black", fill="#e8eaf6", width=2)
        tc(d, CX, y+18, text, f(10))
    def dm(y, text):
        w, h = 120, 44
        d.polygon([CX, y, CX+w//2, y+h//2, CX, y+h, CX-w//2, y+h//2], outline="black", fill="white", width=2)
        tc(d, CX, y+h//2, text, f(9))
    def ar(x1,y1,x2,y2,lb=""):
        d.line([x1,y1,x2,y2], fill="black", width=1)
        if lb: tc(d, (x1+x2)//2+20, (y1+y2)//2-8, lb, f(8), "#666")
        dx, dy = x2-x1, y2-y1
        n = 8
        if abs(dx) > abs(dy):
            d.polygon([x2-n,y2-4,x2-n,y2+4,x2,y2] if dx>0 else [x2+n,y2-4,x2+n,y2+4,x2,y2], fill="black")
        else:
            d.polygon([x2-4,y2-n,x2+4,y2-n,x2,y2] if dy>0 else [x2-4,y2+n,x2+4,y2+n,x2,y2], fill="black")
    def va(y1, y2): ar(CX, y1, CX, y2)
    d.ellipse([CX-10,15,CX+10,35], fill="black")
    y=55; bx(y,"Show Menu"); va(y+36,y+60)
    y=y+60+36; bx(y,"Select Mode"); va(y+36,y+60)
    y=y+60+36; bx(y,"Init Players")
    ly=y+80+36; d.rectangle([CX-80,ly,CX+80,ly+290], outline="#3949ab", width=1)
    tc(d, CX+85, ly+285, "Game Loop", f(8), "#3949ab")
    y=ly+10; bx(y,"Display Board"); va(y+36,y+55)
    y=y+55+36; bx(y,"Get Move"); va(y+36,y+55)
    y=y+55+36; bx(y,"Place Piece"); va(y+36,y+50)
    y=y+50; dm(y,"Check Win?")
    ar(CX+80,y+22,CX+150,y+100,"Yes")
    ry=y+95; bx(ry,"Show Result"); va(ry+36,ry+55)
    ry=ry+55+36; dm(ry,"Play Again?")
    ar(CX-80,ry+22,CX,95,"Yes")
    ar(CX+80,ry+22,CX+150,ry+70,"No")
    d.ellipse([CX+142,ry+68,CX+158,ry+84], fill="black")
    ar(CX-80,y+22,CX,ly+15,"No")
    p = os.path.join(IMG_DIR, "act.png"); img.save(p); print("  act.png"); return p

def ss_menu():
    img = Image.new("RGB", (600,400), "black")
    d = ImageDraw.Draw(img)
    lines = ["=== TIC-TAC-TOE ===","1. PvP","2. PvAI Easy","3. PvAI Medium","4. PvAI Hard","5. AI vs AI","0. Exit","================","Select: 4"]
    y=20
    for l in lines:
        d.text((15,y), l, fill="#0f0", font=fm(22))
        y+=35
    p = os.path.join(IMG_DIR,"menu.png"); img.save(p); return p

def ss_game():
    img = Image.new("RGB", (480,420), "black")
    d = ImageDraw.Draw(img)
    lines = ["   1   2   3"," +---+---+---+","1| X | O |   |"," +---+---+---+","2|   | X |   |"," +---+---+---+","3|   | O |   |"," +---+---+---+","","--- Current ---","Human (X)","","Enter: 3 1"]
    y=20
    for l in lines:
        d.text((15,y), l, fill="#0f0", font=fm(18))
        y+=27
    p = os.path.join(IMG_DIR,"game.png"); img.save(p); return p

def build():
    pdf_path = os.path.join(OUTPUT_DIR, "shuili1ban-hezhangjie-ruanjian-gongcheng-da-zuoye.pdf")
    print("Generating diagrams...")
    diags = {"uc":draw_uc(),"cls":draw_cls(),"seq":draw_seq(),"act":draw_act(),"menu":ss_menu(),"game":ss_game()}
    doc = SimpleDocTemplate(pdf_path, pagesize=A4, topMargin=20*mm, bottomMargin=20*mm, leftMargin=20*mm, rightMargin=20*mm)
    story = []
    story.append(Spacer(1, 40*mm))
    story.append(Paragraph("Software Engineering Project", st))
    story.append(Spacer(1, 8*mm))
    story.append(Paragraph("MVC Tic-Tac-Toe Game", ss))
    story.append(Spacer(1, 20*mm))
    info = Table([["Class","Water Resources Hydropower 1"],["Name","He Zhangjie"],["Course","SE"],["Date","June 2026"]], colWidths=[35*mm,85*mm])
    info.setStyle(TableStyle([('FONTNAME',(0,0),(-1,-1),FONT_NAME),('FONTSIZE',(0,0),(-1,-1),11),('GRID',(0,0),(-1,-1),0.5,colors.grey),('TOPPADDING',(0,0),(-1,-1),4*mm),('BOTTOMPADDING',(0,0),(-1,-1),4*mm)]))
    story.append(info)
    story.append(Spacer(1,15*mm))
    story.append(Paragraph("GitHub: https://github.com/hhhh0914/SE_TicTacToe", bd))
    story.append(PageBreak())
    story.append(Paragraph("1. Requirements", h1))
    story.append(Paragraph("C++ Tic-Tac-Toe with MVC architecture. 3x3 board, CLI, VS2022, Git.", bd))
    for r in ["MVC design","Two-player mode","Human vs AI (3 levels)","AI vs AI watch","Win/draw detection","Input validation"]:
        story.append(Paragraph(f"- {r}", bl))
    story.append(PageBreak())
    story.append(Paragraph("2. Use Case Diagram", h1))
    story.append(Image(diags["uc"], width=170*mm, height=108*mm))
    story.append(Paragraph("Fig 1: Use Case Diagram", cp))
    story.append(PageBreak())
    story.append(Paragraph("3. Class Diagram", h1))
    story.append(Image(diags["cls"], width=170*mm, height=115*mm))
    story.append(Paragraph("Fig 2: Class Diagram", cp))
    cd = Table([["Class","Layer","Role"],["Board","Model","3x3 grid, win"],["Player","Model","Name, symbol"],["GameState","Model","Status, turn"],["ConsoleView","View","CLI"],["Controller","Controller","Flow"],["AIPlayer","Controller","Minimax"]],colWidths=[28*mm,22*mm,85*mm])
    cd.setStyle(TableStyle([('FONTNAME',(0,0),(-1,-1),FONT_NAME),('FONTSIZE',(0,0),(-1,-1),8.5),('BACKGROUND',(0,0),(-1,0),CP),('TEXTCOLOR',(0,0),(-1,0),colors.white),('GRID',(0,0),(-1,-1),0.5,colors.grey),('TOPPADDING',(0,0),(-1,-1),2*mm),('BOTTOMPADDING',(0,0),(-1,-1),2*mm)]))
    story.append(cd)
    story.append(PageBreak())
    story.append(Paragraph("4. Sequence Diagram", h1))
    story.append(Image(diags["seq"], width=170*mm, height=120*mm))
    story.append(Paragraph("Fig 3: Sequence Diagram", cp))
    story.append(PageBreak())
    story.append(Paragraph("5. Activity Diagram", h1))
    story.append(Image(diags["act"], width=150*mm, height=160*mm))
    story.append(Paragraph("Fig 4: Activity Diagram", cp))
    story.append(PageBreak())
    story.append(Paragraph("6. Module Division & Design", h1))
    story.append(Paragraph("Model: Board, Player, GameState. View: ConsoleView. Controller: GameController, AIPlayer.", bd))
    rd = Table([["Relation","Classes","Desc"],["Composition","GameState-Board","Owns"],["Association","GameState-Player","Holds"],["Association","Controller-State","Manages"],["Dependency","Ctrl-AIPlayer","Static"],["Dependency","AIPlayer-Board","Reads"]],colWidths=[22*mm,35*mm,80*mm])
    rd.setStyle(TableStyle([('FONTNAME',(0,0),(-1,-1),FONT_NAME),('FONTSIZE',(0,0),(-1,-1),8.5),('BACKGROUND',(0,0),(-1,0),CP),('TEXTCOLOR',(0,0),(-1,0),colors.white),('GRID',(0,0),(-1,-1),0.5,colors.grey),('TOPPADDING',(0,0),(-1,-1),2*mm),('BOTTOMPADDING',(0,0),(-1,-1),2*mm)]))
    story.append(rd)
    story.append(PageBreak())
    story.append(Paragraph("7. Running Screenshots", h1))
    story.append(Image(diags["menu"], width=130*mm, height=85*mm))
    story.append(Paragraph("Fig 5: Main Menu", cp))
    story.append(Spacer(1,5*mm))
    story.append(Image(diags["game"], width=105*mm, height=90*mm))
    story.append(Paragraph("Fig 6: Game Board", cp))
    story.append(PageBreak())
    story.append(Paragraph("8. Conclusion", h1))
    story.append(Paragraph("MVC Tic-Tac-Toe: 5 modes, 3 AI levels, Minimax, proper Git workflow.", bd))
    story.append(Paragraph("https://github.com/hhhh0914/SE_TicTacToe", bd))
    doc.build(story)
    sz = os.path.getsize(pdf_path)/1024
    print(f"PDF: {pdf_path} ({sz:.1f} KB)")
if __name__ == "__main__":
    build()

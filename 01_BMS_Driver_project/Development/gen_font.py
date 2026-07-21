# -*- coding: utf-8 -*-
"""生成 16x16 OLED 中文字模 (纵向取模, 低位在前, 阴码)
格式与 Int_OLED_ShowChinese 一致: 32 字节 = 上半页16列 + 下半页16列
"""
from PIL import Image, ImageFont, ImageDraw

FONT_PATH = r"C:\Windows\Fonts\simsun.ttc"
SIZE = 16

def glyph_bytes(ch, font_px=16, dx=0, dy=1):
    img = Image.new("1", (SIZE, SIZE), 0)
    font = ImageFont.truetype(FONT_PATH, font_px)
    draw = ImageDraw.Draw(img)
    # 居中微调
    bbox = draw.textbbox((0, 0), ch, font=font)
    w = bbox[2] - bbox[0]
    h = bbox[3] - bbox[1]
    ox = (SIZE - w) // 2 - bbox[0] + dx
    oy = (SIZE - h) // 2 - bbox[1] + dy
    draw.text((ox, oy), ch, fill=1, font=font)

    px = img.load()
    out = []
    # 两页: page0 = 上8行, page1 = 下8行
    for page in range(2):
        for col in range(16):
            b = 0
            for bit in range(8):
                row = page * 8 + bit
                if px[col, row]:
                    b |= (1 << bit)   # 低位在前: bit0=顶部
            out.append(b)
    return out

def fmt(ch, comment_idx):
    data = glyph_bytes(ch)
    hexs = ", ".join("0x%02X" % b for b in data)
    return "    {%s}, /*\"%s\",%d*/" % (hexs, ch, comment_idx)

if __name__ == "__main__":
    import sys

    # 自检: 生成已知字 "目" 与原字库比对, 一致才说明取模格式正确
    ref_mu = [0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,0x00,
              0x00,0x00,0xFF,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0xFF,0x00,0x00,0x00]
    ok = glyph_bytes("目") == ref_mu
    print("== 自检 目: %s ==" % ("通过 OK" if ok else "不一致! 检查字体/dy"))

    # 用法: python gen_font.py 要显示的字 [起始索引]
    #   例: python gen_font.py 电压电流 8
    if len(sys.argv) < 2:
        print("用法: python gen_font.py <汉字> [起始索引]")
        print("例:   python gen_font.py 电压电流 8")
        sys.exit(0)

    text = sys.argv[1]
    start = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    print("-- 复制下面几行到 Int_OLED_FONT.h 的 Hzk1[] 中 --")
    for i, ch in enumerate(text, start=start):
        print(fmt(ch, i))


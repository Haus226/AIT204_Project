from PIL import Image, ImageColor
import glob, os




os.chdir(os.path.dirname(os.path.abspath(__file__)))

def saveMap(file, output):
    map_ = []
    with open(file) as f:
        for l in f.readlines()[:len(f.readlines()) - 1]:
            map_.append(list(l[:len(l) - 1]))

    rowNum = len(map_)
    colNum = len(map_[0])
    image = Image.new("RGB", (colNum, rowNum))
    cal = 0
    for r in range(len(map_)):
        for c in range(len(map_[r])):
            if map_[r][c] == "S":
                image.putpixel((c, r), (0, 255, 0))
                cal += 1
            elif map_[r][c] == "T":
                image.putpixel((c, r), (255, 0, 0))
                cal += 1
            elif map_[r][c] == "W":
                image.putpixel((c, r), (0, 0, 0))
            elif map_[r][c] == "N":
                image.putpixel((c, r), (0, 0, 255))
            elif map_[r][c] == "C":
                image.putpixel((c, r), (125, 125, 125))
                cal += 1
            elif map_[r][c] == "R":
                image.putpixel((c, r), (255, 255, 0))
                cal += 1
            elif map_[r][c] == ".":
                image.putpixel((c, r), (255, 255, 255))
    image.save(output)
    # print(file_name, " Calculated : ", cal)
    # print(f"{output} saved successfully !") 


for f in glob.glob("output/*.txt"):
    file_name = os.path.basename(f)
    base_name, _ = os.path.splitext(file_name)
    new_file_name = base_name + '.png'
    saveMap(f, f"outputImage/{new_file_name}")

print("Visualization Done...")

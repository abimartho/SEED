import board
import time
import adafruit_character_lcd.character_lcd_i2c as character_lcd

cols = 16
rows = 2

i2c = board.I2C()
lcd = character_lcd.Character_LCD_I2C(i2c, cols, rows)

def initializeLCD():
    lcd.clear()
    lcd.color = [0, 0, 100]
    lcd.message = "CLYDE"
    
def writeLCD(sentMessage):
    lcd.message = str(sentMessage)
    
def lcdOff():
    lcd.color = [0,0,0]
    lcd.clear()
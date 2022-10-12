import board
import busio
import adafruit_character_lcd.character_lcd_i2c as character_lcd

cols = 16
rows = 2


def initializeLCD():
    i2c = busio.I2C(board.SCL, board.SDA)
    lcd = character_lcd.Character_LCD_I2C(i2c, cols, rows)
    lcd.clear()
    lcd.color = [0, 0, 100]
    
def writeLCD(message):
    lcd.message = str(message)
    
def lcdOff():
    lcd.color = [0,0,0]
    lcd.clear()
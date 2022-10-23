import board
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

lcd_cols = 16
lcd_rows = 2

i2c = board.I2C() # uses board.SCL and board.SDA
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_cols, lcd_rows)

def initializeLCD():
    lcd.clear()
    lcd.color = [0, 0, 100]
    lcd.message = "CLYDE"
    
def writeLCD(sentMessage):
    lcd.message = str(sentMessage)
    
def lcdOff():
    lcd.color = [0,0,0]
    lcd.clear()


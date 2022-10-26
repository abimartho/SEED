import board #Necessary Package to Communicate with LCD using I2C
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd #Library that contains necessary functions to write and control LCD

#Defines LCD Layout
lcd_cols = 16
lcd_rows = 2

#Initializes I2C Communication with PI and LCD
i2c = board.I2C() # uses board.SCL and board.SDA
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_cols, lcd_rows)

#When called clears the LCD of any existing information and sets the color. Displays CLYDE to test message functionality.
def initializeLCD():
    lcd.clear()
    lcd.color = [0, 0, 100]
    lcd.message = "CLYDE"
   
#WriteLCD takes in any parameter and converts it to a string to write to LCD.
def writeLCD(sentMessage):
    lcd.message = str(sentMessage)

#lcdOff is used in scenarios when we want to stop the code in a controlled manner and will clear and turn off the LCD so upon next usage it is ready.
def lcdOff():
    lcd.color = [0,0,0]
    lcd.clear()


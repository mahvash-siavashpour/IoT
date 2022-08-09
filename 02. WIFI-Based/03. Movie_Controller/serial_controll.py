import serial, time
import vlc
import time

media_player = vlc.MediaPlayer()
media = vlc.Media("3.mkv")
media_player.set_media(media)

# set size of video
media_player.video_set_scale(0.6)
# setting volume
volume = 50
media_player.audio_set_volume(volume)

# close serial monitor in arduino IDE
ser = serial.Serial('COM3', 9600)

while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        print(decoded_bytes)
        if decoded_bytes == 'Play':
            media_player.play()
        if decoded_bytes == "Stop":
            media_player.pause()
        if decoded_bytes == "Up": 
            volume += 20
            media_player.audio_set_volume(volume)
        if decoded_bytes == "Down":
            volume -= 20
            media_player.audio_set_volume(volume) 
    except Exception as E:
        print(E)
        break
    time.sleep(1)
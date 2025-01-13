"""

File: Aoifes_BDay.py

Author: Me

Happy birthday Aoife.
Glad I chose team Piano <3
- Sean

"""

import pygame
import mido
from mido import MidiFile
import serial
import serial.tools.list_ports as port_list
import time
from pathlib import Path
import random
import threading
from threading import Thread

class MiniPiano():
    def __init__(self):
        self.noOfKeys = 0
        self.noOfNotes = 0
        self.soundType = "piano"
        # key list
        self.keys = []
        self.notes = []
        self.exit = False
        #score for game...
        self.left_player = 0
        self.right_player = 0
                        # C  C# D  D# E  F  F# G  G# A  A# B
        self.key_light = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        # C  C# D  D# E  F  F# G  G# A  A# B
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        self.viewPorts()
        self.connected = False

        while not self.connected:
            try:
                comPortNum = input("Enter the number for your com port: ")
                self.serialComm = serial.Serial(f"COM{comPortNum}", 115200)
                self.connected = True
            except Exception as e:
                print(f"An excetion occured: {e}")
                print(f"Could not connect to COM{comPortNum}")

    def viewPorts(self):
        ports = list(port_list.comports())
        print("Available Serial ports:")
        for port in ports:
            print(port)

    def setScale(self, scale_select):

            if (scale_select >= 1) and (scale_select <= 6):
                if scale_select == 1:
                    scale_select = 0
                elif scale_select == 2:
                    scale_select = 12
                elif scale_select == 3:
                    scale_select = 24
                elif scale_select == 4:
                    scale_select = 36
                elif scale_select == 5:
                    scale_select = 48
                elif scale_select == 6:
                    scale_select = 60

                print(scale_select)
            else:
                print("Invalid Scale Input")

    def updateLEDs(self):
        send_string = ''.join(str(key) for key in self.key_light)
        self.serialComm.write(send_string.encode())
        time.sleep(0.05)

    def loop_LEDs(self):
        print("LED thread started")
        while not self.exit:
            self.updateLEDs()

    def exit_loop(self):
            input("\nPress to exit!!!")
            for i, keys in enumerate(self.key_light):
                self.key_light[i] = 0
            time.sleep(0.1)
            self.exit = True

    def parseSongMidi(self, music_file):
        """
        stream music with mixer.music module in blocking manner
        this will stream the sound from disk while playing
        """
        clock = pygame.time.Clock()
        midiSong = MidiFile(music_file)

        self.exit = False

        try:
            pygame.mixer.music.load(str(music_file))
        except pygame.error:
            print ("File %s not found! (%s)") # (music_file, pygame.get_error())
            return

        threading.Thread(target=self.loop_LEDs, daemon=True).start()
        threading.Thread(target=self.exit_loop, daemon=True).start()

        try:
            mid = mido.MidiFile(music_file)

            # Determine the scale based on the highest note in the MIDI file
            max_note = max(msg.note for msg in mid if msg.type in ['note_on', 'note_off'])
            scale = (max_note // 12) - 2
            print(f"Detected scale: {scale}")

            pygame.mixer.music.play()
            # while pygame.mixer.music.get_busy():
            #     # check if playback has finished
            #     clock.tick(30)
            #
            #     if self.exit == True:
            #         pygame.mixer.music.stop()
            #         break
            for msg in mid.play():
                # midioutPort.send(msg) # play sound regardless
                # print(msg)
                if(msg.type in ['note_on','note_off']):
                    if msg.note:
                        octave = msg.note//12
                        # print(f"Octave: {octave}")
                        if scale == octave:
                            key = msg.note%12
                        elif scale == octave - 1:
                            key = msg.note%12 + 12

                        if(msg.type == 'note_on' and msg.velocity > 0):
                            self.key_light[key] = 1
                        elif(msg.type == 'note_off' or msg.velocity == 0):
                            self.key_light[key] = 0

                        # print(msg.note)
                        # if(msg.type == 'note_on' and msg.velocity > 0):
                        #     key.selfPlayActive()
                        # elif(msg.type == 'note_off' or msg.velocity == 0):
                        #     key.selfPlayStop()

                if self.exit:
                    pygame.mixer.music.stop()
                    break

            for i, keys in enumerate(self.key_light):
                    self.key_light[i] = 0
            time.sleep(0.1)
            self.exit = True

        except Exception as e:
            print(f"Error Playing {midiSong} :(")
            print(e)

        # while pygame.mixer.music.get_busy():
        #     print(pygame.mixer.music.get_busy())
        #     # check if playback has finished
        #     clock.tick(30)

    def playPiano(self):
        while(1):
            print("What game would you like to play:")
            print("\nOptions are:")
            print("\t3   ->     Self Play Song - Print out List and Enter Index")
            print("\t4   ->     Chopsticks Game - Just plays the Midi File :)")
            print("\t5   ->     Self Play Random Song")
            print("\t51  ->     Self Play Random Song - Aoife List")
            print("\t52  ->     Self Play Random Song - Sean List - Still just the office...")
            print("\t53  ->     Self Play Select Song - Select Few")
            print("\t54  ->     Self Play Random Song - Select Few")
            print("\t55  ->     Self Play Random Song - Difficulty Choice")
            print("\t0   ->     END THE PROGRAM")

            try:
                game = int(input("Enter your choice: "))
            except ValueError:
                print("Invalid input! Please enter a number.")
                pass

            if game == 3:
                print("\n ")
                songs = list(Path("midi_songs/").glob('*.mid'))
                songs.sort()
                for i, song in enumerate(songs):
                    song = song.name.replace(".mid", "")
                    song_name = song.replace("MIDI File - ", "")
                    songs[i] = song_name
                print("\nWhat song would you like to play:")

                # Display the songs with indices
                for i, song in enumerate(songs):
                    try:
                        if i:
                            print(f"Number: {i}, Song: {song}")
                    except ValueError:
                        print("Something wrong with the file name")
                        continue

                # User selects a song by index
                song_index = int(input("Enter the song number: "))
                selected_song = songs[song_index-1]
                print(selected_song)
                midiSong = f"midi_songs/MIDI File - {selected_song}.mid"
                print("Time to play: " + midiSong)
                pygame.mixer.music.set_volume(0.8)
                self.parseSongMidi(midiSong)

            elif game == 4:
                pygame.mixer.music.set_volume(1.2)
                print("\n ************ CHOPSTICKS... ********* ")
                self.parseSongMidi("TimedGame/Chopsticks_Added_Key_Fast.mid")

            elif game == 5:
                print("\n ************ Random Song ********* \n")
                songs = list(Path("midi_songs/").glob('*.mid'))
                songs.sort()
                random_song = random.choice(songs)

                try:
                    print(f"Playing Song: {random_song.name}")
                except ValueError:
                    print("Something wrong with the file name")
                    pass

                pygame.mixer.music.set_volume(0.8)
                self.parseSongMidi(str(random_song))

            elif game == 51:
                print("\n ************ Random Song - Aoife Choice ********* \n")
                selectFewSongs = list(Path("midi_songs/Aoife/").glob('*.mid'))
                song_index = random.choice(selectFewSongs)

                self.parseSongMidi(str(song_index))

            elif game == 52:
                print("\n ************ Random Song - Sean Choice ********* \n")
                selectFewSongs = list(Path("midi_songs/Sean/").glob('*.mid'))
                song_index = random.choice(selectFewSongs)

                self.parseSongMidi(str(song_index))

            elif game == 53:
                print("\n ")
                print("\n ************ Select Song - Select Few ********* \n")
                selectFewSongs = list(Path("midi_songs/SelectFew/").glob('*.mid'))
                selectFewSongs.sort()

                print("\nWhat song would you like to play:")

                # Display the songs with indices
                for i, song in enumerate(selectFewSongs):
                    try:
                        print(f"Number: {i}, Song: {song.name}")
                    except ValueError:
                        print("Something wrong with the file name")
                        continue

                # User selects a song by index
                song_index = int(input("Enter the song number: "))
                selected_song = selectFewSongs[song_index]

                midiSong = str(selected_song)
                print("Time to play: " + midiSong)
                pygame.mixer.music.set_volume(0.8)
                self.parseSongMidi(midiSong)

            elif game == 54:
                print("\n ************ Random Song - Select Few ********* \n")
                selectFewSongs = list(Path("midi_songs/SelectFew/").glob('*.mid'))

                song_index = random.choice(selectFewSongs)

                try:
                    print(f"Number: {song_index}, Song: {str(song_index)}")
                except ValueError:
                    print("Something wrong with the file name")
                    pass

                pygame.mixer.music.set_volume(0.8)
                self.parseSongMidi(str(song_index))

            elif game == 55:
                print("\n ")
                print(" ************ Random Song - Difficulty Choice ********* ")
                print("\n ")
                songsFiltered = []

                try:
                    print("What difficulty level song would you like to play:")
                    print("\nOptions are:")
                    print("\t1  -> Easy")
                    print("\t2  -> Medium")
                    print("\t3  -> Intermediate")
                    print("\t4  -> Hard")
                    print("\t5  -> Advanced")
                    difficulty = int(input("Enter your Difficulty Choice: "))
                except ValueError:
                    print("\nInvalid input!")
                    pass

                if difficulty == 1:
                    print("\nChose Easy")
                    songsFiltered = list(Path("midi_songs/Easy/").glob('*.mid'))
                elif difficulty == 2:
                    print("\nChose Medium")
                    songsFiltered = list(Path("midi_songs/Medium/").glob('*.mid'))
                elif difficulty == 3:
                    print("\nChose Intermediate")
                    songsFiltered = list(Path("midi_songs/Intermediate/").glob('*.mid'))
                elif difficulty == 4:
                    print("\nChose Hard")
                    songsFiltered = list(Path("midi_songs/Hard/").glob('*.mid'))
                elif difficulty == 5:
                    print("\nChose Advanced")
                    songsFiltered = list(Path("midi_songs/Advanced/").glob('*.mid'))
                else:
                    print("Invalid Choice... Chose all songs instead...!")
                    songsFiltered = list(Path("midi_songs/").glob('*.mid'))

                print("\n List of Songs... :")
                for i,song in enumerate(songsFiltered):
                    try:
                        print(f"Number: {i}, Song: {song.name}")
                    except ValueError:
                        print("Something wrong with the file name")
                        continue
                print("\nDone")

                random_song = random.choice(songsFiltered)
                try:
                    print(f"\n\tPlaying Song: {random_song.name}")
                except ValueError:
                    print("Something wrong with the file name...")
                    pass

                midiSong = str(random_song)
                pygame.mixer.music.set_volume(0.8)
                self.parseSongMidi(midiSong)

            elif game == 0:
                print("\nExiting the program.")
                break
            else:
                print("\nInvalid Input!!! Try again :)")


mini_piano = MiniPiano()


music_file_name = "midi_songs/MIDI File - The Office - Theme.mid"

# convert back to a binary midi and save to a file in the working directory
# fish = base64.b64decode(mid64)
# fout = open(music_file,"wb")
# fout.write(fish)
# fout.close()

freq = 44100    # audio CD quality
bitsize = -16   # unsigned 16 bit
channels = 2    # 1 is mono, 2 is stereo
buffer = 1024    # number of samples
pygame.mixer.init(freq, bitsize, channels, buffer)

# optional volume 0 to 1.0
pygame.mixer.music.set_volume(0.8)

try:
    mini_piano.playPiano()

    # Old
    #mini_piano.parseSongMidi(music_file_name)
except KeyboardInterrupt:
    # if user hits Ctrl/C then exit
    # (works only in console mode)
    pygame.mixer.music.fadeout(1000)
    pygame.mixer.music.stop()
    raise SystemExit

# gv-kbd

Linux virtual keyboard for typing using gamepads

This project aims to build a fully working virtual keyboard for Linux gamers. The concept and look is based on the Idea of Charlie Deck that you can find here : https://www.gamedeveloper.com/design/towards-better-gamepad-text-input

## Build instruction :

1. clone the repository
2. make sure you have the SDL input and display system libraries installed.
3. "cd" to the gv-kbd folder
4. run "make"
5. run the binary from the folder "./prog"

## How to use :

1. Connect your xbox controller (maping are hardcoded currently, so other gamepad may not work but you can try)
2. run the binary from the gv-kbd folder "./prog"
3. you can write some text in the terminal window by choosing letter with the left/right analog joystick and select by pressing the left/right analog shoulder

## To-Do

I have very few leisure time because of my study. So, updates will takes longer, if you are intested in the project and want to contribute, you can email me to pisento@eterny.fr or send me message to my matrix account @pisento1:pisento.tk. So you are free to fork this project

- [ ] add uinput support for typing text outside the terminal window
- [ ] add gamepad user mapping rather than hardcoded mapping (for users who don't own a xbox controller)
- [ ] custom sensibility and area selection
- [ ] backspace − enter − shift keys
- [ ] add non qwerty layout (especially azerty layout for french users)
- [ ] create beautiful assets instead of shamely using a screen capture of the https://slashcards.com/#_=_ game asset

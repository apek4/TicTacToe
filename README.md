# TicTacToe

2-player Tic-Tac-Toe Game using Nexys A7 FPGA board

![image](https://github.com/apek4/TicTacToe/assets/114537462/13a74c58-1aca-4738-a922-a0424e76ef9e)

Software and Hardware requirements:

Hardware:
  1. Diligent Nexys A7 FPGA board connected to computer via MicroUSB cable. 
  2. VGA-compliant monitor and VGA cable to connect your board.

Software
  1. Xilinx Vivado (Bitstream generation)
  2. Keil uVision (Software build)
  3. Tera Term (Keyboard input)

Steps to run projects:

1. Connect Nexys A7 board to the PC using USB and a Monitor using VGA cable
2. Open Vivado Project:
    a) Program and Debug -> Open Target -> Autoconnect
    b) Program and Debug -> Program Target -> 'choose device' -> Load Bitstream file(Vivado/VivadoProject/reflash.bit) -> Program
3. Open TeraTerm
    Setup -> Serial Port -> set Baud rate:19200 -> New setting
    Follow instructions displayed in the text console of VGA Monitor to play the game
    
    

from state import States
import serial

Port = serial.Serial('/dev/cu.SLAB_USBtoUART',115200)
counter = 32 # below 32 everything in ASCII is gibberish

def isValidState(state):
    return state in States

def sendState(state):
    Port.write(state.encode())
    Port.flush()
    Port.write(str(States[state]).encode())

def prompt():
    print "\nWelcome to the 2016 US Presidential Election State By State Visualizer ... \n"
    while True :
        state = raw_input("Enter a state initial. Eg. [CA or TX] or 'q' to quit: ")
        if state == "q":
            return
        elif isValidState(state) != True :
            print "\n",state, "is not a valid state initial. Please try Again\n"
        else :
            print "\n2016 Election Results for", state
            sendState(state)
            print "\n----------------------------------------------------------------------\n"

prompt()

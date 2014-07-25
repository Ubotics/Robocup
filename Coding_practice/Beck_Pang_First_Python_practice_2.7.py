"""
Beck Pang 25/07/2014
First practice project for our summer robotics team
"""

import random

def name_to_number (name):
    # This helper function converts the string name into a number between 0 an 4
    # pre: take a name in String as a parameter
    # post: return an represented number in integer
    if (name == "rock"):
        return 0
    elif (name == "Spock"):
        return 1
    elif (name == "paper"):
        return 2
    elif (name == "lizard"):
        return 3
    elif (name == "scissors"):
        return 4
    else:
        print ("This name is not included in this game.\n")

def number_to_name (number):
    # pre: take a number in integer as a parameter
    # post: return a name in String
    if (number == 0):
        return "rock"
    elif (number == 1):
        return "Spock"
    elif (number == 2):
        return "paper"
    elif (number == 3):
        return "lizard"
    elif (number == 4):
        return "scissors"
    else:
        return "no word found"

def rpsls (player_choice):
    # This function operate the main functionality
    # pre: take a player's choice in String as a parameter
    # post: print the player and computer's choices in the console
    #       and show the result
    print ("\n")
    player_number = name_to_number(player_choice)
    comp_number = random.randrange(5)
    comp_name = number_to_name(comp_number)
    print ("Player chooses " + player_choice + "\n")
    print ("Computer chooses " + comp_name + "\n")
    difference = (comp_number - player_number) % 5
    if (difference == 0):
        print ("Player and computer tie!")
    elif (difference >= 2):
        print ("Player wins!")
    else:
        print ("Computer wins!")


"""  There is no main function in this game
     Please play this game in the console.
     """


    

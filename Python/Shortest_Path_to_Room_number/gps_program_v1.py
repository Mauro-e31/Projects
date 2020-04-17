
import csv
import queue
import time
import os


import functions as f_helper
from functions import tiles
from functions import Node


"""
NOTE:
floor_1_dimension = [177, 293]  # start = 178, 21
floor_2_dimension = [318, 390]  # A1:NZ318 start = r =166 c = 47
floor_3_dimension = [337, 330]  # A1:LR337 r = 165 c = 89
floor_4_dimension = [331, 406]  # A1:OP331 r =331 c = 406 s_start = r174,c39
floor_5_dimension = [155, 400]  # A1:OJ155  r=57 c= 40
"""












def get_directions_in_a_list( room_number):

    room_number = int(room_number)#int(input("what room number would you like to go to ? "))



    floor_destination = f_helper.get_floor_dest(room_number)
    file_name = f_helper.get_name_of_floor_file(room_number)
    #coordinates = f_helper.get_starting_r_c(room_number)
    r = 177
    c = 293
    mypaths= f_helper.my_GPS_two(room_number)
    my_path2 = mypaths[0]
    my_path = mypaths[1]
    main_map=[]
    f_helper.load_map_from_file_gps(main_map,c,r,file_name)


    print("*************Directions for room # " +
        str(room_number) + "********************")
    #my_path2= ["forward","forward","forward","back","back","back"]

    if(room_number > 200):
        directions2 = f_helper.get_direction_commands(my_path2)


    directions = f_helper.get_direction_commands(my_path)

    if(room_number > 200):
        print(*directions2)
        print("\n**********TAKE ELEVATOR TO " + floor_destination + "*********\n")

    print(*directions)



    #os.system("cls||clear")
    """ This is the beginning of the real-life update of the gps mappping system"""
    ################################################################################
    ###**********************************************************************#######


    instructions = list(f_helper.get_raw_direction_commands(my_path))
    return instructions,main_map


#FOR TESTING PURPOSE#
def get_directions_in_a_list_testing( room_number):

    room_number = int(room_number)#int(input("what room number would you like to go to ? "))
    main_map,my_path=f_helper.get_path_in_list_test(room_number)
    #print("*************Directions for room # " + str(room_number) + "********************")

    directions = f_helper.get_direction_commands(my_path)

    #print(*directions)


    instructions = list(f_helper.get_raw_direction_commands(my_path))
    return instructions,main_map
#END OF TESTING#

#NOTE:automatic following #
def my_main():
    #NOTE:for testing only
    #ask what floor youre currently on

    room_number =input("what room number would you like to go to ? ")

    main_map,my_path = f_helper.get_path_in_list_test(int(room_number))
    directions = f_helper.get_direction_commands(my_path)
    main_map=[]
    f_helper.load_map_from_file_gps(main_map,0,0,"zac_floor_schematics/third_floor.csv")
    print(*directions)
    f_helper.print_mini_map(main_map, 188, 91)  # prints a minimap-
    """
    instructions,main_map= get_directions_in_a_list(input("what room number would you like to go to ? ") )
    print("Starting gps system\n\n")
    start_r = 177
    start_c = 20
    f_helper.print_mini_map(main_map, start_r, start_c)  # prints a minimap-

    #loop_count = 1
    while( (len(instructions) >0) ):
        distance_left = float(instructions[0])
        direction_to_go = str(instructions[1])

        while(distance_left>0):
            os.system("cls||clear")
            #if(loop_count%3==0):
                #os.system("cls||clear")
            #command1= str(input("first command "))
            
            if( (direction_to_go=="forward")):
                start_c= start_c+1
                distance_left= distance_left-1.25

            if((direction_to_go=="backwards")):
                start_c= start_c-1
                distance_left= distance_left-1.25
            if(  (direction_to_go=="left")):
                start_r= start_r-1
                distance_left= distance_left-1.25
            if(  (direction_to_go=="right")):
                start_r= start_r+1
                distance_left= distance_left-1.25

            #if(command1=="q"):
                #break
            #print(start_c,start_r)
            print(distance_left,direction_to_go)
            f_helper.print_mini_map(main_map,start_r,start_c)
            time.sleep(.3)
        
        instructions= instructions[2:]
        print(instructions)
        """
    #NOTE MANUAL
    """
    while((len(instructions) > 0)):
        distance_left = float(instructions[0])
        direction_to_go = str(instructions[1])

        while(distance_left > 0):
            if(loop_count % 3 == 0):
                os.system("cls||clear")
            command1 = str(input("first command "))
            if((command1 == "r") and (direction_to_go == "forward")):
                start_c = start_c+1
                distance_left = distance_left-1.25

            elif(command1 == "l" and direction_to_go == "backwards"):
                start_c = start_c-1
                distance_left = distance_left-1.25
            elif(command1 == "u" and direction_to_go == "left"):
                start_r = start_r-1
                distance_left = distance_left-1.25
            elif(command1 == "d" and direction_to_go == "right"):
                start_r = start_r+1
                distance_left = distance_left-1.25

            elif(command1 == "q"):
                break
            
            else:
                os.system("cls||clear")
                print("*********-----RE-ROUTING---********")

            # print(start_c,start_r)
            print(distance_left, direction_to_go)
            f_helper.print_mini_map(main_map, start_r, start_c)

        instructions = instructions[2:]
        print(instructions)
    """





if __name__=="__main__":
    my_main()
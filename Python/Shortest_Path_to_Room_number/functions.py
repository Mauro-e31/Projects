import csv
import queue
import math
"""******CLASSES****"""
#NOTE: this class is in a way the rover. This class is used as a way to store * 
#* all possible instances that can lead up to a solution. Ex: if the rover goes right, * 
#* this class will store an updated map, and add it to the path, and put it to the queue, 
#* this way we can check all possible locations. 
class Node:
    def __init__(self,value,row=0,collumn=0,map_copy=[]):
        #self.value= int(value) ##changing it to a float
        #*this value will be how far it is from the target
        self.value= float(value)
        self.rlocation= int(row)
        self.clocation= int(collumn)
        self.childrens=[]
        self.map=map_copy
        self.path_taken=[]


    def add_child(self,child):
        self.childrens.append(child)

    def __str__(self):
        return str(self.childrens)

#////////////////////////////////
#NOTE: tile class is where the rover will be standing on *
#* this will allow me to store a value such as a wall(1), a open space (0) * 
#* or if it has already been visited 
class tiles:
    def __init__(self,value):
        self.value= int(value)
        self.visited = False
    
    def __str__(self):
        return str(self.visited)


##****************END---OF---CLASSES---***************#######
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##



##****************BEGINNING--OF--FUNCTIONS---*********#######



"""FUNCTIONS"""
def load_map_from_file(matrix_,c,r):
    with open('map_test2.csv', newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) >c:
                extra = len(row)- c
                new_row= row
                for n in range(extra):
                    new_row.pop()
                
                matrix_.append(new_row)

            #main_map.append(row)
            #print(*row)


##***//loading the raw matrix data into a 2-D array//**##
def load_map_from_file_gps(matrix_,c,r,file_name):
    with open(file_name, newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            matrix_.append(row)
            #main_map.append(row)
            #print(*row)

##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##
def print_matrix( matrix_ ):
    for i in range( len(matrix_ ) ):
        for j in range ( len( matrix_[i] ) ):
            print('{:<2}'.format(matrix_[i][j] ) + " ",end='')
        print()
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##

def print_map( matrix_ ):
    for i in range( len(matrix_ ) ):
        for j in range ( len( matrix_[i] ) ):
            print('{:<2}'.format(matrix_[i][j].value ) + " ",end='')
        print()
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##
"""Printing an updated version of the map to help keep track of the rover"""

def print_mini_map(matrix_,r,c):
    #Fixing a good matrix size 
    max_left_c=0
    max_right_c =0

    max_up_r = 0
    max_down_r= 0

    max_size=10
    count = max_size

    #left
    while (count >=0):
        if(c-count>=0):
            max_left_c = c-count
            break
        count = count-1
    #end of left
    
    #Right
    count = max_size
    while (count >= 0):
        if(c+count <= len(matrix_)):
            max_right_c = c+count
            break
        count = count-1
    #end of right
    count = max_size
    #up 
    while (count >=0):
        if(c-count>=0):
            max_up_r = r-count
            break
        count = count-1
    #end of up 
    count = max_size
    #down 
    while (count >= 0):
        if(c+count <= len(matrix_)):
            max_down_r = r+count
            break
        count = count-1
    #end of down

    for i in range(max_up_r,max_down_r) :
        for j in range(max_left_c,max_right_c):
            if( (i==r) & (j==c) ):
                print('{:<2}'.format( "*") + " ",end='')
            else: 
                print('{:<2}'.format(matrix_[i][j] ) + " ",end='')
        print()
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##

def valid_move( row,collumn,matrix_):
    #print(len(matrix_)," <--- len matrix" )
    """
    print("checking validation ")
    print(row,collumn)
    
    print("len(matrix)-1= ",len(matrix_)-1)
    print("leng_matrix[row]-1= ",len(matrix_[row])-1)"""

    if row > len( matrix_)-1 or row<0:
        #print("condition1")
        return False
    if (collumn> len( matrix_[row])-1 or collumn<0 ):
        #print("condition2")
        return False
    if matrix_[row][collumn].value == 1:
        #print("value is 1 ")
        #print("condition3")
        return False
    if matrix_[row][collumn].visited == True:
        #print("condition4")
        return False
    
    else :
        return True
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##


def create_tile_map(matrix_):
    new_map=[]
    for i in range( len(matrix_) ):
        tiles_row =[]
        for j in range(len(matrix_[i])):
            new_tile = tiles(matrix_[i][j])
            tiles_row.append(new_tile)
    
        new_map.append(tiles_row)
    
    return new_map
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##

def get_direction_commands(matrix_):
    directions =[]
    previous_value =matrix_[0]
    scaled = 1.125
    total_ = scaled #default =1
    for i in range( 1,len (matrix_) ):
        #print(matrix_[i+1],"= value ")
        if matrix_[i]== previous_value:
            total_=total_+scaled
            #print(i)
            if i == len(matrix_)-1:
                directions.append(total_)
                directions.append("Ft.")
                directions.append(str(previous_value) + " -->")
                previous_value=matrix_[i]

        elif i==len(matrix_)-1:
            directions.append(total_)
            directions.append("Ft.")
            directions.append(str(previous_value)+" -->")
            total_=float(scaled)

            directions.append(total_)
            directions.append("Ft.")
            directions.append(str(matrix_[i])+ " -->")

        else:
            #print("here --",matrix_[i])
            directions.append(total_)
            directions.append("Ft.")
            directions.append(str(previous_value)+ " -->")
            previous_value=matrix_[i]
            total_=float(scaled)

    
    return directions
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##

def get_raw_direction_commands(matrix_):
    directions =[]
    previous_value =matrix_[0]
    scaled=1.125
    total_ = scaled #default =1
    for i in range( 1,len (matrix_) ):
        #print(matrix_[i+1],"= value ")
        if matrix_[i]== previous_value:
            total_=total_+scaled
            #print(i)
            if i == len(matrix_)-1:
                directions.append(total_)
                """directions.append("Ft.")"""
                directions.append(str(previous_value) )
                previous_value=matrix_[i]

        elif i==len(matrix_)-1:
            directions.append(total_)
            """directions.append("Ft.")"""
            directions.append(str(previous_value))
            total_=float(scaled)

            directions.append(total_)
            """directions.append("Ft.")"""
            directions.append(str(matrix_[i]))

        else:
            #print("here --",matrix_[i])
            directions.append(total_)
            """directions.append("Ft.")"""
            directions.append(str(previous_value))
            previous_value=matrix_[i]
            total_=float(scaled)

    
    return directions
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##

def check_for_highway(map1,row,collumn):
        count=int(0)
        #right
        if valid_move(row,collumn+1,map1):
            if map1[row][collumn+1].value==2:
                count= count+1
        #left
        if valid_move(row,collumn-1,map1):
            if map1[row][collumn-1].value==2:
                count= count+1

        #down
        if valid_move(row+1,collumn,map1):
            if map1[row+1][collumn].value==2:
                count= count+1

        #up
        if valid_move(row-1,collumn,map1):
            if map1[row-1][collumn].value==2:
                count= count+1
        return count

#function to find the distance from goal#
#using pythagoream theorem#

def distance_from_goal(current_r,current_c,goal_r,goal_c):
    total_l = float( (goal_r-current_r)+1 ) 
    total_w = float( (goal_c-current_c)+1)

    total_distance = math.sqrt( math.pow(total_l,2)+ math.pow(total_w,2) )
    return (float(total_distance) )


######NOTE: TESTING NEWWWWWWWWW SHORTESSSSSSSSTT PATH #####################
##****************************************************************##

def find_shortest_path3(start_r,start_c, room_number, map2):
    my_queue= queue.Queue() # *NOTE
    #*this is the main queu, the only queue that will expand the given node * 
    #* I am going to be using two queues or list, that will sort the items based on * 
    #* how far they are from our goal, and ONLY expand the one closer to the goal * 
    
    possible_routes = queue.Queue() #stores possible routes the rover can take
    # initial_distance_away = distance_from_goal(start_r,start_c)
    root = Node(0)
    root.rlocation = start_r  # start
    root.clocation = start_c
    map2[start_r][start_c].visited = True
    root.map = map2

    my_queue.put(root)

    
    my_path = []


    while my_queue.qsize() > 0 or (not possible_routes.empty() ): #checking the main queue
        #temp_node=Node(0)
        if(not my_queue.empty() ):
            temp_node = my_queue.get()
            #print("here")
        else:
            temp_node= possible_routes.get()
        #temp_node = my_queue.get()
        map1 = temp_node.map
        row = int(temp_node.rlocation)
        collumn = int(temp_node.clocation)
        temp_value = int(temp_node.value)
        temp_path = list(temp_node.path_taken)

        temp_room_number = map1[row][collumn].value


        if temp_room_number == room_number:
            #print("found the destination")
            my_path = temp_node.path_taken
            break

        temp_high_ways_found = int(check_for_highway(map1, row, collumn))

        if temp_high_ways_found >= 1:
            #go forward
            #print("found highway")
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                if map1[row][collumn+1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                    #temp_node.add_child(forward_child)
                    forward_child.path_taken = list(temp_path)
                    forward_child.path_taken.append("forward")
                    my_queue.put(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                if map1[row][collumn-1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    back_child = Node(temp_value+1, row, collumn-1, map_copy)
                    #temp_node.add_child(back_child)
                    back_child.path_taken = list(temp_path)
                    back_child.path_taken.append("back")
                    my_queue.put(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                if map1[row+1][collumn].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    right_child = Node(temp_value+1, row+1, collumn, map_copy)
                    #temp_node.add_child(right_child)
                    right_child.path_taken = list(temp_path)
                    right_child.path_taken.append("right")
                    my_queue.put(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                if map1[row-1][collumn].value == 2:

                    #print("can go left")
                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    left_child = Node(temp_value+1, row-1, collumn, map_copy)
                    #temp_node.add_child(left_child)
                    left_child.path_taken = list(temp_path)
                    left_child.path_taken.append("left")
                    my_queue.put(left_child)

        #end of highway implementation
        elif temp_high_ways_found and (map1[row][collumn]==2) <1:
            #print(highway_flag)
            #print("here ")
            map_copy = map1
            map_copy[row][collumn].visited = False
            #temp_node.add_child(left_child)
            #my_queue.put(temp_node)
            possible_routes.put(temp_node)

        else:

            #go forward
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                #temp_node.add_child(forward_child)
                forward_child.path_taken = list(temp_path)
                forward_child.path_taken.append("forward")
                possible_routes.put(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                back_child = Node(temp_value+1, row, collumn-1, map_copy)
                #temp_node.add_child(back_child)
                back_child.path_taken = list(temp_path)
                back_child.path_taken.append("back")
                possible_routes.put(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                right_child = Node(temp_value+1, row+1, collumn, map_copy)
                #temp_node.add_child(right_child)
                right_child.path_taken = list(temp_path)
                right_child.path_taken.append("right")
                possible_routes.put(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                #print("can go left")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                left_child = Node(temp_value+1, row-1, collumn, map_copy)
                #temp_node.add_child(left_child)
                left_child.path_taken = list(temp_path)
                left_child.path_taken.append("left")
                possible_routes.put(left_child)

    return my_path


########******END OF TESTING***********#################################


##GET FLOOR DESTINATION ##
def get_floor_dest(room_number):
    floor_destination = "FIRST FLOOR"

    if(room_number > 200 and room_number < 300):  # Second Floor
        floor_destination = "SECOND FLOOR "
    elif(room_number > 300 and room_number < 400):  # Third Floor
        floor_destination = "THIRD FLOOR "
    elif(room_number > 400 and room_number < 500):  # Fourth Floor
        floor_destination = "FOURTH FLOOR "
    elif(room_number > 500 and room_number < 600):  # Fifth Floor
        floor_destination = "FIFTH FLOOR "
    return floor_destination

#************************************####

def get_name_of_floor_file(room_number):
    file_name2= "zac_floor_schematics/first_floor.csv"
    if(room_number > 200 and room_number < 300):    #Second Floor
        file_name2 = "zac_floor_schematics/second_floor.csv"
    elif(room_number > 300 and room_number < 400):  #Third Floor
        file_name2 = "zac_floor_schematics/third_floor.csv"
    elif(room_number > 400 and room_number < 500):  #Fourth Floor
        file_name2 = "zac_floor_schematics/fourth_floor.csv"
    elif(room_number > 500 and room_number < 600):  #Fifth Floor
        file_name2 = "zac_floor_schematics/fifth_floor.csv"
    return file_name2

def get_starting_r_c(room_number):
    coordinates = []
    if(room_number > 200 and room_number < 300):    #Second Floor
        r = 165
        c = 45
        #r=187
        #c=50
        coordinates.append(int(r) )
        coordinates.append(int(c) ) 
    elif(room_number > 300 and room_number < 400):  #Third Floor
        r = 164
        c = 88
        coordinates.append(int ( 164) )
        coordinates.append(int (88) ) 
    elif(room_number > 400 and room_number < 500):  #Fourth Floor
        r = 173
        c = 38
        coordinates.append( int (173 ) ) 
        coordinates.append( int (38) )
    elif(room_number > 500 and room_number < 600):  #Fifth Floor
        r = 56
        c = 39
        coordinates.append( int (56) ) 
        coordinates.append( int (39) ) 
    return r,c


def my_GPS(room_number):
    r = 177 #default, we start at first floor
    c = 293 #""#

    my_path = []            #start with an empy path
    my_path2 = []           #path2 is used, if the user goes above first floor
    file_name2 = ""         #empty file name for now
    

    if(room_number > 200):
        #************************************#
        file_name = "/first_floor.csvfirst_floor.csv"
        main_map_t = []
        load_map_from_file_gps(main_map_t, c, r, file_name)
        new_map = create_tile_map(main_map_t)
        #my_path2 = f_helper.find_shortest_path(177, 20, -1, new_map)
        my_path2 = find_shortest_path3(177, 20, -1, new_map) #elevator location
        #**************************************#
        #NOTE: Since the user is going above the first floor * 
        # * we need to take the elevator, meaning we need to *
        # * make that our priority, so we first find the shortest* 
        # * path to the elevator, and then added to our path from * 
        # * elevator to our room goal*

        main_map_t = []

        load_map_from_file_gps(main_map_t, c, r, file_name2)
        new_map = create_tile_map(main_map_t)
        #my_path = f_helper.find_shortest_path(r, c, room_number, new_map)
        my_path = find_shortest_path3(r, c, room_number, new_map)
    else:
        file_name = "zac_floor_schematics/first_floor.csv"
        main_map_t = []
        load_map_from_file_gps(main_map_t, c, r, file_name)
        new_map = create_tile_map(main_map_t)
        my_path = find_shortest_path3(177, 20, room_number, new_map)

    mypaths=[]
    mypaths.append(list (my_path2))
    mypaths.append(list(my_path))
    return mypaths


def my_GPS_two(room_number):
    r = 177 #default, we start at first floor
    c = 293 #""#
    floor_destination = get_floor_dest(room_number)
    file_name_of_floor= get_name_of_floor_file(room_number)



    my_path = []            #start with an empy path
    my_path2 = []           #path2 is used, if the user goes above first floor
    file_name2 = file_name_of_floor


    if(room_number > 200):
        #************************************#
        file_name = "zac_floor_schematics/first_floor.csv"
        main_map_t = []
        load_map_from_file_gps(main_map_t, c, r, file_name)
        new_map = create_tile_map(main_map_t)
        #my_path2 = f_helper.find_shortest_path(177, 20, -1, new_map)
        my_path2 = find_shortest_path3(177, 20, -1, new_map) #elevator location
        #**************************************#
        #NOTE: Since the user is going above the first floor * 
        # * we need to take the elevator, meaning we need to *
        # * make that our priority, so we first find the shortest* 
        # * path to the elevator, and then added to our path from * 
        # * elevator to our room goal*

        main_map_t = []
        load_map_from_file_gps(main_map_t, c, r, file_name2)
        new_map = create_tile_map(main_map_t)
        #my_path = f_helper.find_shortest_path(r, c, room_number, new_map)
        r,c= get_starting_r_c(room_number)
        my_path = find_shortest_path3(r, c, room_number, new_map)
    else:
        file_name = "zac_floor_schematics/first_floor.csv"
        main_map_t = []
        load_map_from_file_gps(main_map_t, c, r, file_name)
        new_map = create_tile_map(main_map_t)
        my_path = find_shortest_path3(177, 20, room_number, new_map)

    mypaths=[]
    mypaths.append(list (my_path2))
    mypaths.append(list(my_path))
    return mypaths


def path_in_third_floor(room_number):
    print("testing")
    floor_destination = get_floor_dest(room_number)
    file_name_of_floor= get_name_of_floor_file(room_number)
    file_name2 = file_name_of_floor
    r,c= get_starting_r_c(room_number)
    main_map_t = []
    load_map_from_file_gps(main_map_t, c, r, file_name2)
    new_map = create_tile_map(main_map_t)
    #my_path = f_helper.find_shortest_path(r, c, room_number, new_map)
    r,c= get_starting_r_c(room_number)
    r=188
    c=91
    my_path = find_shortest_path3(r, c, room_number, new_map)

    return new_map,my_path


def get_path_in_list_test(room_number):
    file_name_of_floor= get_name_of_floor_file(room_number)
    file_name2 = file_name_of_floor
    r,c= get_starting_r_c(room_number)
    main_map_t = []
    load_map_from_file_gps(main_map_t, c, r, file_name2)
    new_map = create_tile_map(main_map_t)
    #my_path = f_helper.find_shortest_path(r, c, room_number, new_map)
    r,c= get_starting_r_c(room_number)
    r=188
    c=91
    my_path = find_shortest_path3(r, c, room_number, new_map)

    return main_map_t,my_path
































































##NOTE: extra code that could be use but for now is ignored 
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##
"""
#MAIN ALGORITHM: 

def find_shortest_path(start_r,start_c, room_number, map2):
    my_queue= queue.Queue()
    root = Node(0)
    root.rlocation = start_r  # start
    root.clocation = start_c
    map2[start_r][start_c].visited = True
    root.map = map2

    my_queue.put(root)

    
    my_path = []

    highway_flag = 0

    while my_queue.qsize() > 0:
        temp_node = my_queue.get()
        map1 = temp_node.map
        row = int(temp_node.rlocation)
        collumn = int(temp_node.clocation)
        temp_value = int(temp_node.value)
        temp_path = list(temp_node.path_taken)

        temp_room_number = map1[row][collumn].value

        #print(highway_flag)
        #print(temp_value)

        #highway_tiles = int(0)
        #print("------------------back to queue----------------")
        #print("current location = ",end='')
        #print(row,collumn)

        if temp_room_number == room_number:
            #print("found the destination")
            my_path = temp_node.path_taken
            break

        if map1[row][collumn].value == 2:
            #print(highway_flag)
            highway_flag = highway_flag-1

        #implement priority highway
        #print(check_for_highway(map1,row,collumn))
        temp_high_ways_found = int(check_for_highway(map1, row, collumn))
        highway_flag = highway_flag + temp_high_ways_found

        if temp_high_ways_found >= 1:
            #go forward
            #print("found highway")
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                if map1[row][collumn+1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                    #temp_node.add_child(forward_child)
                    forward_child.path_taken = list(temp_path)
                    forward_child.path_taken.append("forward")
                    my_queue.put(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                if map1[row][collumn-1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    back_child = Node(temp_value+1, row, collumn-1, map_copy)
                    #temp_node.add_child(back_child)
                    back_child.path_taken = list(temp_path)
                    back_child.path_taken.append("back")
                    my_queue.put(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                if map1[row+1][collumn].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    right_child = Node(temp_value+1, row+1, collumn, map_copy)
                    #temp_node.add_child(right_child)
                    right_child.path_taken = list(temp_path)
                    right_child.path_taken.append("right")
                    my_queue.put(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                if map1[row-1][collumn].value == 2:

                    #print("can go left")
                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    left_child = Node(temp_value+1, row-1, collumn, map_copy)
                    #temp_node.add_child(left_child)
                    left_child.path_taken = list(temp_path)
                    left_child.path_taken.append("left")
                    my_queue.put(left_child)

        #end of highway implementation
        elif highway_flag > -200000:
            #print(highway_flag)
            #print("here ")
            map_copy = map1
            map_copy[row][collumn].visited = False
            #temp_node.add_child(left_child)
            my_queue.put(temp_node)

        else:

            #go forward
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                #temp_node.add_child(forward_child)
                forward_child.path_taken = list(temp_path)
                forward_child.path_taken.append("forward")
                my_queue.put(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                back_child = Node(temp_value+1, row, collumn-1, map_copy)
                #temp_node.add_child(back_child)
                back_child.path_taken = list(temp_path)
                back_child.path_taken.append("back")
                my_queue.put(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                right_child = Node(temp_value+1, row+1, collumn, map_copy)
                #temp_node.add_child(right_child)
                right_child.path_taken = list(temp_path)
                right_child.path_taken.append("right")
                my_queue.put(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                #print("can go left")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                left_child = Node(temp_value+1, row-1, collumn, map_copy)
                #temp_node.add_child(left_child)
                left_child.path_taken = list(temp_path)
                left_child.path_taken.append("left")
                my_queue.put(left_child)

    return my_path
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##
def find_shortest_path2(start_r,start_c, room_number, map2):
    my_queue= list()
    root = Node(0)
    root.rlocation = start_r  # start
    root.clocation = start_c
    map2[start_r][start_c].visited = True
    root.map = map2

    my_queue.append(root)

    
    my_path = []

    highway_flag = 0

    while len(my_queue) > 0:
        temp_node = my_queue.pop()
        map1 = temp_node.map
        row = int(temp_node.rlocation)
        collumn = int(temp_node.clocation)
        temp_value = int(temp_node.value)
        temp_path = list(temp_node.path_taken)

        temp_room_number = map1[row][collumn].value

        #print(highway_flag)
        #print(temp_value)

        #highway_tiles = int(0)
        #print("------------------back to queue----------------")
        #print("current location = ",end='')
        #print(row,collumn)

        if temp_room_number == room_number:
            #print("found the destination")
            my_path = temp_node.path_taken
            break

        if map1[row][collumn].value == 2:
            #print(highway_flag)
            highway_flag = highway_flag-1

        #implement priority highway
        #print(check_for_highway(map1,row,collumn))
        temp_high_ways_found = int(check_for_highway(map1, row, collumn))
        highway_flag = highway_flag + temp_high_ways_found

        if temp_high_ways_found >= 1:
            #go forward
            #print("found highway")
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                if map1[row][collumn+1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                    #temp_node.add_child(forward_child)
                    forward_child.path_taken = list(temp_path)
                    forward_child.path_taken.append("forward")
                    my_queue.append(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                if map1[row][collumn-1].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    back_child = Node(temp_value+1, row, collumn-1, map_copy)
                    #temp_node.add_child(back_child)
                    back_child.path_taken = list(temp_path)
                    back_child.path_taken.append("back")
                    my_queue.append(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                if map1[row+1][collumn].value == 2:

                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    right_child = Node(temp_value+1, row+1, collumn, map_copy)
                    #temp_node.add_child(right_child)
                    right_child.path_taken = list(temp_path)
                    right_child.path_taken.append("right")
                    my_queue.append(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                if map1[row-1][collumn].value == 2:

                    #print("can go left")
                    map_copy = list(map1)
                    map_copy[row][collumn].visited = True
                    left_child = Node(temp_value+1, row-1, collumn, map_copy)
                    #temp_node.add_child(left_child)
                    left_child.path_taken = list(temp_path)
                    left_child.path_taken.append("left")
                    my_queue.append(left_child)

        #end of highway implementation
        elif highway_flag > -60000:
            #print(highway_flag)
            #print("here ")
            map_copy = map1
            map_copy[row][collumn].visited = False
            #temp_node.add_child(left_child)
            my_queue.append(temp_node)

        else:

            #go forward
            if valid_move(row, collumn+1, map1):
                #print("can go forward ")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                forward_child = Node(temp_value+1, row, collumn+1, map_copy)
                #temp_node.add_child(forward_child)
                forward_child.path_taken = list(temp_path)
                forward_child.path_taken.append("forward")
                my_queue.append(forward_child)
            #go back
            if valid_move(row, collumn-1, map1):
                #print("can go back")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                back_child = Node(temp_value+1, row, collumn-1, map_copy)
                #temp_node.add_child(back_child)
                back_child.path_taken = list(temp_path)
                back_child.path_taken.append("back")
                my_queue.append(back_child)

            #go right
            if valid_move(row+1, collumn, map1):
                # print("can go right")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                right_child = Node(temp_value+1, row+1, collumn, map_copy)
                #temp_node.add_child(right_child)
                right_child.path_taken = list(temp_path)
                right_child.path_taken.append("right")
                my_queue.append(right_child)
            #go left
            if valid_move(row-1, collumn, map1):
                #print("can go left")
                map_copy = list(map1)
                map_copy[row][collumn].visited = True
                left_child = Node(temp_value+1, row-1, collumn, map_copy)
                #temp_node.add_child(left_child)
                left_child.path_taken = list(temp_path)
                left_child.path_taken.append("left")
                my_queue.append(left_child)

    return my_path
##/////////////////////////////////////////////////////////##
##/////////////////////////////////////////////////////////##
"""

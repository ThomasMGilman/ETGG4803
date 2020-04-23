import math
import random
from operator import itemgetter

# map
'''
Size:
    W: 1000
    H: 1000
    
Layout:
(0,0)---------------------------------------------
|                                                 |
|                                                 |
|                 ------------                    |
|                                                 |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|         |                           |           |
|                                                 |
|                 ------------                    |
|                                                 |
|                                                 |
 ------------------------------------------(999,999)
'''

tank_barrier_offset = 5


def get_angle_d(x1, y1, x2, y2):
    angle = -((180 / math.pi) * math.atan2(y2 - y1, x2 - x1))
    if angle < 0:
        offset = 180 + angle
        angle = 180 + offset

    return int(angle)


def get_distance(x1, y1, x2, y2):
    return math.fabs(math.sqrt((x1 - x2)**2 + (y1 - y2)**2))


def collide_boundaries(x, y, angle):
    """Check if colliding with world boundaries"""
    if angle == 0 and x + 25 >= 999 - tank_barrier_offset:      # move right
        return True
    elif angle == 90 and y - 25 <= tank_barrier_offset:         # move up
        return True
    elif angle == 180 and x - 25 <= tank_barrier_offset:        # move left
        return True
    elif angle == 270 and y + 25 >= 999 - tank_barrier_offset:  # move down
        return True
    else:
        return False


class Barrier(object):
    def __init__(self, min_x, min_y, max_x, max_y):
        self.min_x = min_x
        self.min_y = min_y
        self.max_x = max_x
        self.max_y = max_y

    def get_distance_min_min(self, otherX, otherY):
        return math.fabs(math.sqrt((self.min_x - otherX) ** 2 + (self.min_y - otherY) ** 2))

    def get_distance_min_max(self, otherX, otherY):
        return math.fabs(math.sqrt((self.min_x - otherX) ** 2 + (self.max_y - otherY) ** 2))

    def get_distance_max_min(self, otherX, otherY):
        return math.fabs(math.sqrt((self.max_x - otherX) ** 2 + (self.min_y - otherY) ** 2))

    def get_distance_max_max(self, otherX, otherY):
        return math.fabs(math.sqrt((self.max_x - otherX) ** 2 + (self.max_y - otherY) ** 2))

    def run_into(self, otherX, otherY, angle):
        if angle == 0 or angle == 180:
            if otherY + 25 >= self.min_y and otherY - 25 <= self.max_y:
                if (angle == 0 and otherX + 25 >= self.min_x - tank_barrier_offset) or \
                        (angle == 180 and otherX - 25 <= self.max_x + tank_barrier_offset):
                    return True
        elif angle == 90 or angle == 270:
            if otherX + 25 >= self.min_x and otherX - 25 <= self.max_x:
                if(angle == 90 and otherY - 25 <= self.max_y + tank_barrier_offset) or \
                        (angle == 270 and otherY + 25 >= self.min_y - tank_barrier_offset):
                    return True
        return False

    '''This is slower than not_blocking probably because of more angle boolean checking'''
    def angle_will_not_hit(self, otherX, otherY, fire_angle, dis):
        """ Barrier:
        (0,0)
                            Above
                               90
                        ------ |------
                        |      |      |
                        |      |      |
                        |      |      |
               Left 180 _______________ 0 Right
                        |      |      |
                        |      |      |
                        |      |      |
                        ------ |------
                              270
                              Below             (999,999)
        """
        # ------------------------# check firing away from barrier
        if otherX > self.max_x and (fire_angle <= 90 or fire_angle >= 270): # Right
            return True
        elif otherX < self.min_x and (90 <= fire_angle <= 270):             # Left
            return True
        elif otherY < self.min_y and (0 <= fire_angle <= 180):              # Above
            return True
        elif otherY > self.max_y and (180 <= fire_angle <= 360):            # Below
            return True

        # ------------------------# Right of Barrier
        if otherX > self.max_x:
            if otherY > self.max_y:     # Below Barrier
                if 90 > fire_angle > 0:        # Firing Above
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.min_y)
                    if edge_angle > fire_angle:
                        return True
                elif 90 < fire_angle < 180:    # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.max_y)
                    if edge_angle < fire_angle:
                        return True
            elif otherY < self.min_y:   # Above Barrier
                if 270 < fire_angle < 360:     # Firing Down
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.max_y)
                    if edge_angle < fire_angle:
                        return True
                elif 180 < fire_angle < 270:   # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.min_y)
                    if edge_angle < fire_angle:
                        return True
            else:                       # Between Y Edges
                if 180 > fire_angle > 90:    # Firing Above
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.min_y)
                    if edge_angle > fire_angle:
                        return True
                elif 180 < fire_angle < 270: # Firing Below
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.max_y)
                    if edge_angle < fire_angle:
                        return True
                elif dis < (otherX - self.max_x):
                    return True
        #------------------------# Left of Barrier
        elif otherX < self.min_x:
            if otherY > self.max_y:     # Below Barrier
                if 90 > fire_angle > 0:      # Firing Right
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.max_y)
                    if edge_angle > fire_angle:
                        return True
                elif 90 < fire_angle < 180:  # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.min_y)
                    if edge_angle < fire_angle:
                        return True
            elif otherY < self.min_y:   # Above Barrier
                if 90 > fire_angle > 0:      # Firing Right
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.min_y)
                    if edge_angle < fire_angle:
                        return True
                elif 270 < fire_angle < 360: # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.max_y)
                    if edge_angle > fire_angle:
                        return True
            else:                       # Between Y Edges
                if 90 > fire_angle > 0:      # Firing Above
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.min_y)
                    if edge_angle < fire_angle:
                        return True
                elif 270 < fire_angle < 360: # Firing Below
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.max_y)
                    if edge_angle > fire_angle:
                        return True
                elif dis < (self.min_x - otherX):
                    return True
        # ------------------------# Between X Edges
        else:
            if otherY < self.min_y:     # Above Barrier
                if 270 < fire_angle < 360:   # Firing Right
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.min_y)
                    if edge_angle < fire_angle:
                        return True
                elif 270 > fire_angle > 180: # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.min_y)
                    if edge_angle > fire_angle:
                        return True
                elif dis < (self.min_y - otherY):
                    return True
            elif otherY > self.max_y:   # Below Barrier
                if 90 > fire_angle > 0:      # Firing Right
                    edge_angle = get_angle_d(otherX, otherY, self.max_x, self.max_y)
                    if edge_angle > fire_angle:
                        return True
                elif 90 < fire_angle < 180:  # Firing Left
                    edge_angle = get_angle_d(otherX, otherY, self.min_x, self.max_y)
                    if edge_angle < fire_angle:
                        return True
                elif dis < (otherY - self.max_y):
                    return True
        # ------------------------# Will hit Barrier
        return False

    def not_blocking(self, tank_x, tank_y, other_tank_x, other_tank_y, fire_angle):
        # first check if barrier not in line of tanks
        # if both right, both left, both up, or both down = not blocked
        if (other_tank_x > self.max_x and tank_x > self.max_x) or \
                (other_tank_x < self.min_x and tank_x < self.min_x) or \
                (other_tank_y > self.max_y and tank_y > self.max_y) or \
                (other_tank_y < self.min_y and tank_y < self.min_y):
            return True

        left = False
        right = False
        above = False
        below = False

        # barriers corner pos to check
        barrier_x = 0
        barrier_y = 0
        # ----------------------------------------------------- barrier X corner
        # ---------------------------- Left of the barrier
        if tank_x < self.min_x:
            if tank_y > self.max_y:  # Bottom Corner
                if other_tank_x > self.max_x:
                    right = True
                    barrier_x = self.max_x
                    barrier_y = self.max_y
                else:
                    left = True
                    barrier_x = self.min_x
                    barrier_y = self.min_y

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle < corner_angle_from_tank and right) or \
                        (fire_angle > corner_angle_from_tank and left):
                    return True
                else:
                    return False

            elif tank_y < self.min_y:  # Top Corner
                if other_tank_x > self.max_x:
                    right = True
                    barrier_x = self.max_x
                    barrier_y = self.min_y
                else:
                    left = True
                    barrier_x = self.min_x
                    barrier_y = self.max_y

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle > corner_angle_from_tank and right) or \
                        (fire_angle < corner_angle_from_tank and left):
                    return True
                else:
                    return False

            else:  # In the middle
                barrier_x = self.min_x
                if other_tank_y > self.max_y:  # below
                    below = True
                    barrier_y = self.max_y
                elif other_tank_y < self.min_y:  # above
                    above = True
                    barrier_y = self.min_y
                else:
                    return False

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle > corner_angle_from_tank and above) or \
                        (fire_angle < corner_angle_from_tank and below):
                    return True
                else:
                    return False

        # ---------------------------- Right of the barrier
        elif tank_x > self.max_x:
            if tank_y > self.max_y:  # Bottom Corner
                if other_tank_x < self.min_x:
                    left = True
                    barrier_x = self.min_x
                    barrier_y = self.max_y
                else:
                    right = True
                    barrier_x = self.max_x
                    barrier_y = self.min_y

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle < corner_angle_from_tank and right) or \
                        (fire_angle > corner_angle_from_tank and left):
                    return True
                else:
                    return False

            elif tank_y < self.min_y:  # Top Corner
                if other_tank_x < self.min_x:
                    left = True
                    barrier_x = self.min_x
                    barrier_y = self.min_y
                else:
                    right = True
                    barrier_x = self.max_x
                    barrier_y = self.max_y

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle > corner_angle_from_tank and right) or \
                        (fire_angle < corner_angle_from_tank and left):
                    return True
                else:
                    return False

            else:  # In the middle
                barrier_x = self.max_x
                if other_tank_y > self.max_y:  # below
                    below = True
                    barrier_y = self.max_y
                elif other_tank_y < self.min_y:  # above
                    above = True
                    barrier_y = self.min_y
                else:
                    return False

                corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
                if (fire_angle < corner_angle_from_tank and above) or \
                        (fire_angle > corner_angle_from_tank and below):
                    return True
                else:
                    return False

        # ---------------------------- Above barrier in the middle
        elif tank_y < self.min_y:
            barrier_y = self.min_y
            if other_tank_x > self.max_x:  # Right
                right = True
                barrier_x = self.max_x
            elif other_tank_x < self.min_x:
                left = True
                barrier_x = self.min_x
            else:
                return False
            corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
            if (fire_angle < corner_angle_from_tank and left) or \
                    (fire_angle > corner_angle_from_tank and right):
                return True
            else:
                return False

        # ---------------------------- Bellow barrier in the middle
        elif tank_y > self.max_y:
            barrier_y = self.max_y
            if other_tank_x > self.max_x:  # Right
                barrier_x = self.max_x
            elif other_tank_x < self.min_x:
                barrier_x = self.min_x
            else:
                return False
            corner_angle_from_tank = get_angle_d(tank_x, tank_y, barrier_x, barrier_y)
            if (fire_angle > corner_angle_from_tank and left) or \
                    (fire_angle < corner_angle_from_tank and right):
                return True
            else:
                return False
        else:
            return False


LeftBarrier     = Barrier(200, 300, 230, 700)  # Left
RightBarrier    = Barrier(770, 300, 800, 700)  # Right
BottomBarrier   = Barrier(400, 200, 600, 225)  # Bottom
TopBarrier      = Barrier(400, 775, 600, 800)  # Top


class OtherTank(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.angle = 0

    def update_pos(self, newX, newY):
        self.angle = abs(math.atan2(newY - self.y, newX - self.x))
        self.x = newX
        self.y = newY

    def get_fire_pos(self, distance):
        offset_amount = distance / 8
        x_pos = offset_amount * math.cos(self.angle)
        y_pos = offset_amount * math.sin(self.angle)
        return (self.x + x_pos, self.y + y_pos)

    def get_distance(self, otherX, otherY):
        return math.fabs(math.sqrt((self.x - otherX) ** 2 + (self.y - otherY) ** 2))

    def get_origin_angle(self, otherX, otherY):
        return get_angle_d(otherX, otherY, self.x, self.y)

    def get_fire_angle(self, otherX, otherY, dis):
        fire_coord = self.get_fire_pos(dis)
        return get_angle_d(otherX, otherY, fire_coord[0], fire_coord[1])

    def get_fire_angle(self, otherX, otherY, fireX, fireY):
        return get_angle_d(otherX, otherY, fireX, fireY)


class Tank_AI(object):
    def __init__(self):
        self.name = "Thomas's T-Tank"  # your bot name here
        self.house = "Witherell"  # your house here: one of "Yost", "Witherell", "Lynn", "Skaggs"
        # your class variables follow
        self.x = 0
        self.y = 0
        self.boardSize = (1000, 1000)  # world Width, Height
        self.diameter = 40  # 40 unit diameter for tank
        self.bulletDiameter = 10  # 10 unit diameter for bullet
        self.bulletSpeed = 8  # 8 units per frame
        self.barrierOffset = 5

        self.my_angle = 0
        self.fire_countdown = 0

        # Dictionary of Tanks
        # key: name
        # value: otherTank
        self.tankDict = {}

    def get_fire_angle(self, closestTankList):
        closestTankList.sort(key=itemgetter(1))
        for i in range(len(closestTankList)):
            name = closestTankList[i][0]
            dis = closestTankList[i][1]
            angle = self.tankDict[name].get_origin_angle(self.x, self.y)
            x = self.tankDict[name].x
            y = self.tankDict[name].y

            left_checked = False
            right_checked = False
            up_checked = False
            down_checked = False
            barrier_check = []

            # right of tank
            if x > self.x:
                right_checked = True
                barrier_check.append(RightBarrier.not_blocking(self.x, self.y, x, y, angle))
                if self.x < LeftBarrier.max_x:
                    left_checked = True
                    barrier_check.append(LeftBarrier.not_blocking(self.x, self.y, x, y, angle))

            elif x < self.x:
                left_checked = True
                barrier_check.append(LeftBarrier.not_blocking(self.x, self.y, x, y, angle))
                if self.x > RightBarrier.min_x:
                    right_checked = True
                    barrier_check.append(RightBarrier.not_blocking(self.x, self.y, x, y, angle))

            if y < self.y:
                up_checked = True
                barrier_check.append(TopBarrier.not_blocking(self.x, self.y, x, y, angle))
                if self.y < BottomBarrier.min_y:
                    down_checked = True
                    barrier_check.append(BottomBarrier.not_blocking(self.x, self.y, x, y, angle))

            elif y > self.y:
                down_checked = True
                barrier_check.append(BottomBarrier.not_blocking(self.x, self.y, x, y, angle))
                if self.y < TopBarrier.max_y:
                    up_checked = True
                    barrier_check.append(TopBarrier.not_blocking(self.x, self.y, x, y, angle))

            if all(barrier_check):
                return angle

            # To slow
            '''elif dis > 168:
                fire_coord = self.tankDict[name].get_fire_pos(x, y)
                fire_angle = self.tankDict[name].get_fire_angle(self.x, self.y, fire_coord[0], fire_coord[1])
                fire_ahead_checks = []
                
                if left_checked:
                    fire_ahead_checks.append(LeftBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle))
                if right_checked:
                    fire_ahead_checks.append(
                        RightBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle))
                if up_checked:
                    fire_ahead_checks.append(
                        TopBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle))
                if down_checked:
                    fire_ahead_checks.append(
                        BottomBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle))

                #fire_ahead_checks = [LeftBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle),
                #                     RightBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle),
                #                     TopBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle),
                #                     BottomBarrier.not_blocking(self.x, self.y, fire_coord[0], fire_coord[1], fire_angle)]
                if all(fire_ahead_checks):
                    return fire_angle'''

        return None

    def update_tanks(self, tank_list):
        """Check the Dictionary for tank locations and return closest tanks angle to fire at"""
        closestTankList = []

        for name, house, x, y, health, points in tank_list:
            # set current location for movement checking
            if (x == self.x and y == self.y and name == self.name and house == self.house) or health == 0:
                continue

            # update other tank locations
            if name+house not in self.tankDict:
                # create array of size two for location and direction
                self.tankDict[name+house] = OtherTank(x, y)
            else:
                self.tankDict[name+house].update_pos(x, y)
            dis = self.tankDict[name+house].get_distance(self.x, self.y)
            closestTankList.append((name+house, dis))

        '''closestTankList.sort(key=itemgetter(1))

        for i in range(len(closestTankList)):
            name = closestTankList[i][0]
            dis = closestTankList[i][1]
            angle = self.tankDict[name].get_origin_angle(self.x, self.y)
            x = self.tankDict[name].x
            y = self.tankDict[name].y

            pool = ThreadPool(processes=4)
            barrier_check =[]

            # right of tank
            if x > self.x:
                barrier_check.append(pool.apply_async(RightBarrier.not_blocking, (self.x, self.y, x, y, angle)))
                if self.x < LeftBarrier.max_x:
                    barrier_check.append(pool.apply_async(LeftBarrier.not_blocking, (self.x, self.y, x, y, angle)))

            elif x < self.x:
                barrier_check.append(pool.apply_async(LeftBarrier.not_blocking, (self.x, self.y, x, y, angle)))
                if self.x > RightBarrier.min_x:
                    barrier_check.append(pool.apply_async(RightBarrier.not_blocking, (self.x, self.y, x, y, angle)))

            if y < self.y:
                barrier_check.append(pool.apply_async(TopBarrier.not_blocking, (self.x, self.y, x, y, angle)))
                if self.y > BottomBarrier.min_y:
                    barrier_check.append(
                        pool.apply_async(BottomBarrier.not_blocking, (self.x, self.y, x, y, angle)))

            elif y > self.y:
                barrier_check.append(pool.apply_async(BottomBarrier.not_blocking, (self.x, self.y, x, y, angle)))
                if self.y < TopBarrier.max_y:
                    barrier_check.append(
                        pool.apply_async(TopBarrier.not_blocking, (self.x, self.y, x, y, angle)))

            results = []
            for j in range(len(barrier_check)):
                results.append(barrier_check[j].get())

            if all(results):
                return angle

        return None'''
        return closestTankList

    def make_move(self, your_location, tank_list, bullet_list):
        if self.fire_countdown > 0:
            self.fire_countdown -= 1

        self.x = your_location[0]
        self.y = your_location[1]

        action = "move"
        action_angle = self.my_angle

        closestTankList = self.update_tanks(tank_list)

        if self.fire_countdown == 0:
            fire_angle = self.get_fire_angle(closestTankList)
            if fire_angle is not None and self.fire_countdown == 0:
                action = "fire"
                action_angle = fire_angle
                self.fire_countdown = 15
        if action != "fire":
            barrier_collisions = [LeftBarrier.run_into(self.x, self.y, self.my_angle),
                                  RightBarrier.run_into(self.x, self.y, self.my_angle),
                                  TopBarrier.run_into(self.x, self.y, self.my_angle),
                                  BottomBarrier.run_into(self.x, self.y, self.my_angle),
                                  collide_boundaries(self.x, self.y, self.my_angle)]

            if any(barrier_collisions):
                if self.my_angle == 0:
                    self.my_angle = random.choice([90, 180, 270])
                elif self.my_angle == 90:
                    self.my_angle = random.choice([180, 270, 0])
                elif self.my_angle == 180:
                    self.my_angle = random.choice([90, 0, 270])
                elif self.my_angle == 270:
                    self.my_angle = random.choice([180, 90, 0])
                action_angle = self.my_angle
            elif random.randint(0, 125) == 0:
                self.my_angle = random.choice([90, 180, 270, 0])
                action_angle = self.my_angle


        # return the selected command
        return action, action_angle

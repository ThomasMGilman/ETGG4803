import pygame
import datetime
import random
import numpy as np
from slot_wheel import SlotWheel
from Text import Text

frameRate = 60
text_size = 20
numSimulations = 10

'''These Are the win states possible and their win amounts'''
win_states = {
    'CHERRY': {'CHERRY': {'CHERRY': 3, 'BELL': 2, 'LEMON': 2, 'BAR': 2},
               'BELL': {'BAR': 1, 'BELL': 1, 'LEMON': 1},
               'LEMON': {'BAR': 1, 'BELL': 1, 'LEMON': 1},
               'BAR': {'BAR': 1, 'BELL': 1, 'LEMON': 1}},
    'LEMON': {'LEMON': {'LEMON': 5}},
    'BELL': {'BELL': {'BELL': 15}},
    'BAR': {'BAR': {'BAR': 20}}
}
win_probability = {}
win_chances = {}
probability_payouts = []


def print_win_cases(print_payouts=False):
    symbols = win_chances.items()
    for sym in symbols:
        print("\t" + sym[0] + ":\n\t\tChance:\n\t\t\tFractional: " +
              str(sym[1]) + "\n\t\t\tPercentage: " +
              str(sym[1] * 100) + "%")
    print("-" * 100)
    if print_payouts:
        print("Payouts: ")
        for payout in probability_payouts:
            print(
                "\tSpin: " + payout[1][0] + "\n\tCoins: " + str(payout[1][1]) + "\n\tChance: " + str(
                    payout[0] * 100) + "%")
            print("-" * 100)


def calculate_probability_of_win(num_slots, print_as_calculate=False):
    total_win_prob = 0
    symbols = win_states.items()
    num_symbols = len(symbols)
    num_possibilities = num_symbols ** num_slots
    num_win_states = 0
    print("-" * 100)
    print("Number of possible outcomes:", num_possibilities)
    if print_as_calculate:
        print("Win Cases:")
    for sym in symbols:
        sym_win_prob = 0
        second_symbols = sym[1].items()
        for second_sym in second_symbols:
            third_symbols = second_sym[1].items()
            payout_probability = ((1 / num_symbols) ** 2) * (len(third_symbols) / num_symbols)
            sym_win_prob += payout_probability
            for third_sym in third_symbols:
                spin = sym[0] + ", " + second_sym[0] + ", " + third_sym[0]
                probability_payouts.append(((1/4)**3, (spin, third_sym[1])))
                win_probability[spin] = payout_probability
                num_win_states += 1
        total_win_prob += sym_win_prob
        win_chances[sym[0]] = sym_win_prob
        if print_as_calculate:
            print("\t" + sym[0] + ":\n\t\tChance:\n\t\t\tFractional: " +
                  str(sym_win_prob) + "\n\t\t\tPercentage: " +
                  str(sym_win_prob * 100) + "%")
    print("Number of win states: " + str(num_win_states))
    print("Probability of win: " + str(total_win_prob * 100) + "% chance")
    print("-" * 100)


def calculate_expected_return(coin_amount):
    """Calculate the expected return for the coin value amount put in for a single game."""
    expected_return = 0
    for payout in probability_payouts:
        print()
        expected_return += payout[0] * (payout[1][1] / coin_amount)  # Percentage * (amount_back / coin_amount_put_in)
    print("Expected Payoff Return: " + str(expected_return))


def check_win(ordered_symbols):
    """check_win([string, string, string]) -> bool, int.
    Passed an array of string symbols, the symbols are checked with the win states to see if a win state was reached.
    If a win state was reached, a payout is assigned based on the win, otherwise 0 is returned"""
    first_sym = ordered_symbols[0]
    state = "LOSE"
    amount = 0
    if first_sym in win_states:
        second_sym = ordered_symbols[1]
        if second_sym in win_states[first_sym]:
            third_sym = ordered_symbols[2]
            if third_sym in win_states[first_sym][second_sym]:
                state = "WIN"
                amount = win_states[first_sym][second_sym][third_sym]
    # print(ordered_symbols, state, amount)
    return state, amount


class Scene:
    def __init__(self, win_width, win_height, caption="Honest Slots by. Thomas Gilman", background_colour=(0, 0, 0)):
        self.update_period = frameRate * 2
        self.count_down = 0
        self.coins = 10
        self.num_simulations = numSimulations
        self.wins = 0
        self.spins = 0
        self.win_coin_amount = 0
        self.simulations = []  # (wins, spins, total_coins_won)
        self.slots = []

        self.running = True
        self.simulating = False
        self.window_width = win_width
        self.window_half_width = win_width // 2
        self.window_height = win_height
        self.window_half_height = win_height // 2
        self.screen = pygame.display.set_mode((win_width, win_height))
        pygame.display.set_caption(caption)
        self.background_colour = background_colour

        slot_width, slot_height = 50, 50
        self.setup_slots(3, np.array([self.window_half_width, self.window_half_height]),
                         slot_width, slot_height, "Slot_Images")

        global text_size
        self.state_text = Text(np.array([self.window_half_width, self.window_half_height // 2 - text_size * 2]),
                               "Want to play some slots? Press Q to Simulate", "RosewoodStd-Regular.OTF", text_size,
                               (255, 255, 255))
        self.coin_amount_text = Text(np.array([self.window_half_width, self.window_half_height // 2 - text_size]),
                                     "Coins: " + str(self.coins), "RosewoodStd-Regular.OTF", text_size,
                                     (255, 255, 255))
        self.update()

    def setup_slots(self, num_slots, pos, half_width, half_height, assets_folder,
                    offset_by_x=True, offset_by_y=False):
        """Setup the slots in the Window evenly spaced apart.
        args ar (slots(int), starting_pos(array([x,y])), width(int), height(int), assets_folder(string))"""
        size_spacing = np.array([half_width * 2 if offset_by_x else 0, half_height * 2 if offset_by_y else 0])
        offset = np.array([0, 0]) + (num_slots // 2) * -size_spacing
        for i in range(num_slots):
            self.slots.append(SlotWheel(pos + offset, half_width, half_height, assets_folder))
            offset += size_spacing

    def print_results(self):
        """Print the test results and info from the simulations and reset."""
        wins = []
        spins = []
        win_coin_amount = []
        total_wins = 0
        total_spins = 0
        total_win_coin_amount = 0
        for sim_result in self.simulations:
            wins.append(sim_result[0])
            total_wins += sim_result[0]
            spins.append(sim_result[1])
            total_spins += sim_result[1]
            win_coin_amount.append(sim_result[2])
            total_win_coin_amount += sim_result[2]
        print("Total Simulations: "+str(numSimulations))
        print("\tTotal Wins: "+str(total_wins))
        print("\tTotal Spins: "+str(total_spins))
        print("\tTotal Coins Won: "+str(total_win_coin_amount))
        print("\tMean win amount: "+str(total_wins/total_spins))
        print("\tMedian win amount: "+str(wins[len(wins) // 2]))
        print("\tMedian spin amount: "+str(spins[len(spins) // 2]))
        self.total_reset()

    def gamble(self):
        """Gamble away your money if you have any, or run the simulation and get the info!!!"""
        if self.coins > 0:
            self.coins -= 1

            symbols = []
            spin = ""
            for slot in self.slots:
                slot.randomly_pick_symbol()
                symbols.append(slot.get_slot_symbol_name())
            state, amount = check_win(symbols)

            self.spins += 1
            if state == "WIN":
                self.wins += 1
            self.win_coin_amount += amount

            self.coins += amount
            self.state_text.update_text("State: " + state)
            self.coin_amount_text.update_text("Coins: " + str(self.coins))
        else:
            self.state_text.update_text("CANT GAMBLE WHAT YOU DON'T HAVE!!! Press R to reset!!!")
            if self.simulating and self.num_simulations > 0:
                self.num_simulations -= 1
                self.simulations.append((self.wins, self.spins, self.win_coin_amount))
            elif self.num_simulations == 0:
                self.print_results()
            self.reset_simulation_vars()

    def total_reset(self):
        self.simulations = []
        self.simulating = False
        self.count_down = 0
        self.num_simulations = numSimulations
        self.reset_simulation_vars()

    def reset_simulation_vars(self):
        """Reset the Simulation variables"""
        self.wins = 0
        self.spins = 0
        self.win_coin_amount = 0
        self.coins = 10
        self.state_text.update_text("Want to play some slots? Press Q to Simulate")
        self.coin_amount_text.update_text("Coins: " + str(self.coins))

    def update(self, dt=0):
        """Update the Application and all its contents"""
        self.handle_input()

        if self.simulating and self.count_down <= 0:
            self.count_down += self.update_period
            self.gamble()
        elif self.simulating:
            self.count_down -= dt

        self.screen.fill(self.background_colour)

        for i in range(len(self.slots)):
            self.slots[i].update(self.screen)

        self.state_text.draw(self.screen)
        self.coin_amount_text.draw(self.screen)

        pygame.display.flip()

    def is_running(self):
        """Check if the current Application is running"""
        return self.running

    def handle_input(self):
        """Handle any input events"""
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    self.running = False
                if event.key == pygame.K_q:
                    self.simulating = not self.simulating
                if event.key == pygame.K_r:
                    self.total_reset()

            elif event.type == pygame.KEYUP:
                if event.key == pygame.K_SPACE:
                    self.gamble()


pygame.init()
mainScene = Scene(500, 250)
random.seed(datetime.datetime.now())
calculate_probability_of_win(3)
print_win_cases(True)
calculate_expected_return(1)

game_clock = pygame.time.Clock()
elapsed = 0
last_elapsed = 0
elapsed_since_last_pass = 0
while mainScene.is_running():
    last_elapsed = elapsed
    elapsed = pygame.time.get_ticks()
    elapsed_since_last_pass = elapsed - last_elapsed

    mainScene.update(elapsed_since_last_pass)
    game_clock.tick(frameRate)

pygame.quit()
quit()

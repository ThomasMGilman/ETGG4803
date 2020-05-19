import math
# Prior Probability ex. sally has flu out of 20 people is P(sally has flu) = 1/20 = 0.05
# Conditional Probability ex. P(Sally has the flu | flu patient is a girl) 1/5 = .20 (as 5 girls in class)
#   else P(sally has the flu | the flue patient is a boy) = 0
'''Bayesian Thereom is P(A|B) = P(B|A) * P(B) / P(A) 
This means that in order to solve this you need to solve for P(B|A) first
this mean probability of B occuring given A. So Probability_of_B / (Probability_of_B + Probability_of_not_B)
'''

def bayesian_probability_two(B_given_A, probA, probB):
    ''' The Probability of a hypothesis 'H'
    Conditional on a new piece of Evidence, 'E'
    Is written as P(H | E)  = (P (E | H) * P(H))  / P(E)'''
    return (B_given_A * probA) / probB


def bayesian_probability(sensitivity, specificity, true_positive):
    a = sensitivity * true_positive
    b = (1 - specificity) * (1 - true_positive)
    return a / (a + b)


def two_norm(x1, y1, x2, y2):
    return math.sqrt((x1-x2)**2 + (y1-y2)**2)


def one_norm(x1, y1, x2, y2):
    return abs(x1 - x2) + abs(y1-y2)



# Times in year Paul and Jason walk on sidewalk
paul_sidewalk_walks = 182.625
jason_sidewalk_walks = 365.25
total_outcomes = paul_sidewalk_walks + jason_sidewalk_walks

# Probability of Jason walking on sidewalk or Paul walking on sidewalk
paul_probability = paul_sidewalk_walks / total_outcomes
jason_probability = jason_sidewalk_walks / total_outcomes

# Chances of wearing a kilt.
# Paul only wears a kilt once every year.
# Jason wears a kilt once every 5 years.
paul_kilt_chance = 1 / 365.25
jason_kilt_chance = 1 / 1826.25     # 365.25 * 5 = 1826.25

# Chance its either or wearing kilt
paul_wearing_kilt = bayesian_probability_two(paul_probability / (paul_kilt_chance + paul_probability), paul_kilt_chance, paul_probability)
jason_wearing_kilt = bayesian_probability_two(jason_probability / (jason_kilt_chance + jason_probability), jason_kilt_chance, jason_probability)

print("Times walk on sidewalk in Year\n\tPaul:", paul_sidewalk_walks, "\n\tJason:", jason_sidewalk_walks)
print("Probability of Paul walking on Sidewalk is: ", paul_probability,
      "\n\tChance of wearing kilt: ", paul_kilt_chance,
      "\n\tChance of walking and wearing kilt:", paul_wearing_kilt)
print("Probability of Jason walking on Sidewalk is: ", jason_probability,
      "\n\tChance of wearing kilt: ", jason_kilt_chance,
      "\n\tChance of walking and wearing kilt:", jason_wearing_kilt)

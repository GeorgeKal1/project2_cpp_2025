import random
import string

# Number of participants (can be changed by students)
NUM_PARTICIPANTS = 100

# Sample country list
COUNTRIES = ["Greece", "USA", "UK", "Germany", "France", "Japan", "China", "Canada", "Brazil", "India", "Mexico", "Argentina", "Equador", "Colombia", "Spain", "Portugal", "Italy", "Poland"]

# Function to generate random names
def random_name():
    first_names = [
        "James", "Mary", "Robert", "Patricia", "John", "Jennifer", "Michael", "Linda", "David", "Elizabeth",
        "William", "Barbara", "Richard", "Susan", "Joseph", "Jessica", "Thomas", "Sarah", "Charles", "Karen",
        "Christopher", "Nancy", "Daniel", "Lisa", "Matthew", "Betty", "Anthony", "Margaret", "Mark", "Sandra",
        "Donald", "Ashley", "Steven", "Kimberly", "Paul", "Emily", "Andrew", "Donna", "Joshua", "Michelle",
        "Kenneth", "Dorothy", "Kevin", "Carol", "Brian", "Amanda", "George", "Melissa", "Edward", "Deborah"
    ]
    
    surnames = [
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
        "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Jackson", "Martin",
        "Lee", "Perez", "Thompson", "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson",
        "Walker", "Young", "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores",
        "Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell", "Carter", "Roberts",
        "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker", "Cruz", "Edwards", "Collins", "Reyes",
        "Stewart", "Morris", "Morales", "Murphy", "Cook", "Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper",
        "Peterson", "Bailey", "Reed", "Kelly", "Howard", "Ramos", "Kim", "Cox", "Ward", "Richardson",
        "Watson", "Brooks", "Chavez", "Wood", "James", "Bennett", "Gray", "Mendoza", "Ruiz", "Hughes",
        "Price", "Alvarez", "Castillo", "Sanders", "Patel", "Myers", "Long", "Ross", "Foster", "Jimenez"
    ]
    
    first_name = random.choice(first_names)
    surname = random.choice(surnames)
    
    return f"{first_name} {surname}"

# Function to generate unique random player IDs and rankings for up to 1,000,000 participants
def generate_unique(n):
    return random.sample(range(1000000), n)  # Ensures unique IDs

# Generate unique IDs for players
player_ids = generate_unique(NUM_PARTICIPANTS)

# Generate unique rankings for players
player_ranks = generate_unique(NUM_PARTICIPANTS)

# Create the input file
data = []
for i in range(NUM_PARTICIPANTS):
    name = random_name()
    player_id = player_ids[i]
    ranking = player_ranks[i]
    country = random.choice(COUNTRIES)
    timestamp = random.randint(1735689600, 1740787199)  # Random UNIX timestamp (January 1, 2025 - February 28, 2025)
    
    data.append(f"{name},{player_id},{ranking},{country},{timestamp}\n")

# Write data to file
with open("contest_participants.txt", "w") as file:
    file.writelines(data)

print(f"File 'contest_participants.txt' with {NUM_PARTICIPANTS} participants created successfully!")

################################################################################
# Game Over - Version 2
# Demonstrates the use of quotes in strings
################################################################################

# Print multiple values
print("Program 'Game Over' 2.0")
print("Same", "message", "as before")
print('Just',
      "a bit",
      "bigger")

# Print triple quoted strings
print(
  """
  _____      ___       ___  ___   _______
 /  ___|    /   |     /   |/   | |   ____| 
 | |       /    |    / /|   /| | |  |__
 | |  _   /  /| |   / / |__/ | | |   __|
 | |_| | /  ___ |  / /       | | |  |____
 \_____//_/   |_| /_/        |_| |_______|

  _____    _     _   ______   ______
 /  _  \  | |   / / | _____| |  _   \\
 | | | |  | |  / /  | |__    | |_|  |
 | | | |  | | / /   |  __|   |  _   /
 | |_| |  | |/ /    | |____  | | \  \\
 \_____/  |___/     |______| |_|  \__\\
  """)

# Sounding the system bell
print("\a")

# Print backslash
print("\t-----------------------------------")
print("\t\t\\\'\" Fancy Credits \"\'\\")
print("\t-----------------------------------")

# Silly strings
print("ABCD" + "EFGH")

# Repeating strings
print("*" * 10)

# Demonstrates numbers and math
print("2000 - 100 + 50 = ", 2000 - 1000 + 50)
print("6 *3 = ", 6 * 3)
print("19 / 4 = ", 19 / 4)
print("19 // 4 = ", 19 // 4)
print("19.0 / 4.0 = ", 19.0 / 4.0)
print("19 % 4  = ", 19 % 4)

# Tuple unpacking
colors = 123, 456, 789
aa, bb, cc = colors
print("color index 0 =", aa)
print("color index 1 =", bb)
print("color index 2 =", cc)
aa = 777
print("color index 0 =", aa)

# Dictionary
animal = {}
animal['fish'] = ['beta', 'tropheus']
animal['cat'] = 'tiger', 'lion'
print("animal = ", animal)
print(animal.get('fish'))
print('dog' in animal)
print("animal's keys = ", animal.keys())
print("animal's values = ", animal.values())
print("animal's items = ", animal.items())

address = {
    '1': {'Name': 'aaa', 'email': 'aaa@gmail.com', 'moble':'010-1111-1111'},
    '2': {'Name': 'bbb', 'email': 'bbb@gmail.com', 'moble':'010-1111-2222'},
    '3': {'Name': 'ccc', 'email': 'ccc@gmail.com', 'moble':'010-1111-3333'},
}
for key in address:
    print("addres's key = ", key, "\tvalue = ", address[key])
for key in address.keys():
    print("addres's key = ", key, "\tvalue = ", address[key])
for key, value in address.items():
    print("addres's key = ", key, "\tvalue = ", value)

# zip
english = 'Monday', 'Tuesday', 'Wednsday'
french = 'Lundi', 'Mardi', 'Mercredi'
list_day = list(zip(english, french))
print(list_day, type(list_day))
dict_day = dict(zip(english, french))
print(dict_day, type(dict_day))
tuple_day = tuple(zip(english, french))
print(tuple_day, type(tuple_day))

# range
for x in range(6, 0, -2):
    print(x)

# character count example
message = '''
The President delivered an astonishing political knifing of the British Prime 
Minister on Thursday, comprehensively undermining her fragile position in 
Britain's tortuous negotiations on leaving the European Union and getting his 
visit to the country off to the most explosive of starts. '''
counter = {}
for c in message:
    counter[c] = counter.setdefault(c, 0) + 1
print(counter)

# Function
def set_value(a, b):
    return b, a
print("Function set_value = ", set_value(10, 20))

# Waiting for the user, this is a nice trick to keep a console window
name = input("\n\nPress the enter key to exit")
print ("Bye~ ", name)

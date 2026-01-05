#!/usr/bin/env python3
"""
Quick validation script for Day 15 C implementation
This verifies the test case calculations match expected results
"""

# Test ingredients
butterscotch = {"capacity": -1, "durability": -2, "flavor": 6, "texture": 3, "calories": 8}
cinnamon = {"capacity": 2, "durability": 3, "flavor": -2, "texture": -1, "calories": 3}

# Part 1 test: 44 butterscotch, 56 cinnamon
print("=== PART 1 VALIDATION ===")
print("Recipe: 44 Butterscotch + 56 Cinnamon")

capacity = 44 * butterscotch["capacity"] + 56 * cinnamon["capacity"]
durability = 44 * butterscotch["durability"] + 56 * cinnamon["durability"]
flavor = 44 * butterscotch["flavor"] + 56 * cinnamon["flavor"]
texture = 44 * butterscotch["texture"] + 56 * cinnamon["texture"]
calories = 44 * butterscotch["calories"] + 56 * cinnamon["calories"]

print(f"  Capacity:   {capacity}")
print(f"  Durability: {durability}")
print(f"  Flavor:     {flavor}")
print(f"  Texture:    {texture}")
print(f"  Calories:   {calories}")

# Set negatives to 0
capacity = max(0, capacity)
durability = max(0, durability)
flavor = max(0, flavor)
texture = max(0, texture)

score = capacity * durability * flavor * texture
print(f"\nScore: {capacity} × {durability} × {flavor} × {texture} = {score}")
print(f"Expected: 62842880")
print(f"Match: {score == 62842880} ✓" if score == 62842880 else f"Match: {score == 62842880} ✗")

# Part 2 test: 40 butterscotch, 60 cinnamon
print("\n=== PART 2 VALIDATION ===")
print("Recipe: 40 Butterscotch + 60 Cinnamon")

capacity = 40 * butterscotch["capacity"] + 60 * cinnamon["capacity"]
durability = 40 * butterscotch["durability"] + 60 * cinnamon["durability"]
flavor = 40 * butterscotch["flavor"] + 60 * cinnamon["flavor"]
texture = 40 * butterscotch["texture"] + 60 * cinnamon["texture"]
calories = 40 * butterscotch["calories"] + 60 * cinnamon["calories"]

print(f"  Capacity:   {capacity}")
print(f"  Durability: {durability}")
print(f"  Flavor:     {flavor}")
print(f"  Texture:    {texture}")
print(f"  Calories:   {calories} (target: 500)")

# Set negatives to 0
capacity = max(0, capacity)
durability = max(0, durability)
flavor = max(0, flavor)
texture = max(0, texture)

score = capacity * durability * flavor * texture
print(f"\nScore: {capacity} × {durability} × {flavor} × {texture} = {score}")
print(f"Expected: 57600000")
print(f"Match: {score == 57600000} ✓" if score == 57600000 else f"Match: {score == 57600000} ✗")

# Test brute force search
print("\n=== BRUTE FORCE SEARCH VALIDATION ===")
best_score_part1 = 0
best_recipe_part1 = None
best_score_part2 = 0
best_recipe_part2 = None

for butterscotch_amt in range(101):
    cinnamon_amt = 100 - butterscotch_amt
    
    capacity = butterscotch_amt * butterscotch["capacity"] + cinnamon_amt * cinnamon["capacity"]
    durability = butterscotch_amt * butterscotch["durability"] + cinnamon_amt * cinnamon["durability"]
    flavor = butterscotch_amt * butterscotch["flavor"] + cinnamon_amt * cinnamon["flavor"]
    texture = butterscotch_amt * butterscotch["texture"] + cinnamon_amt * cinnamon["texture"]
    calories = butterscotch_amt * butterscotch["calories"] + cinnamon_amt * cinnamon["calories"]
    
    # Part 1: no calorie constraint
    cap1 = max(0, capacity)
    dur1 = max(0, durability)
    fla1 = max(0, flavor)
    tex1 = max(0, texture)
    score1 = cap1 * dur1 * fla1 * tex1
    
    if score1 > best_score_part1:
        best_score_part1 = score1
        best_recipe_part1 = (butterscotch_amt, cinnamon_amt)
    
    # Part 2: with 500 calorie constraint
    if calories == 500:
        if score1 > best_score_part2:
            best_score_part2 = score1
            best_recipe_part2 = (butterscotch_amt, cinnamon_amt)

print(f"Part 1 Best: {best_recipe_part1[0]} Butterscotch + {best_recipe_part1[1]} Cinnamon = {best_score_part1}")
print(f"  Expected: 44 Butterscotch + 56 Cinnamon = 62842880")
print(f"  Match: {best_score_part1 == 62842880 and best_recipe_part1 == (44, 56)} ✓" if best_score_part1 == 62842880 and best_recipe_part1 == (44, 56) else f"  Match: {best_score_part1 == 62842880 and best_recipe_part1 == (44, 56)} ✗")

print(f"\nPart 2 Best: {best_recipe_part2[0]} Butterscotch + {best_recipe_part2[1]} Cinnamon = {best_score_part2}")
print(f"  Expected: 40 Butterscotch + 60 Cinnamon = 57600000")
print(f"  Match: {best_score_part2 == 57600000 and best_recipe_part2 == (40, 60)} ✓" if best_score_part2 == 57600000 and best_recipe_part2 == (40, 60) else f"  Match: {best_score_part2 == 57600000 and best_recipe_part2 == (40, 60)} ✗")

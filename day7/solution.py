from collections import Counter

data = """
A Python solution for once!
I just copied the input data here to save some time.
Rank 1203 on Part 1
Rank 4042 on Part 2

I used Python to get the day 6 solution as well, but the code was... much worse.
"""

data = [d.split() for d in data.splitlines()]

def nvalue(n):
    return 0 if n == 'J' else int(n) if n.isdigit() else 10 + 'TQKA'.index(n)

def old_nvalue(n): # added back for posterity (used for part 1)
    return int(n) if n.isdigit() else 10 + 'TJQKA'.index(n)

def cardvalue(card):
    return [nvalue(c) for c in card]

seen = set()
def value(card: str, ignore_joker = False):
    assert type(card) == str and len(card) == 5
    counter = Counter(card)
    if 'J' in card and not ignore_joker:
        cardtype = value(card, True)
        if cardtype[0] == 100:
            return [value(card.replace('J', 'A'))[0]] + [cardvalue(card)]
        if cardtype[0] == 90 or cardtype[0] == 80:
            other = [*set(c for c in card if c != 'J')]
            assert len(other) == 1
            return [value(other[0] * 5)[0]] + [cardvalue(card)]
        if cardtype[0] == 70:
            other = [*set(c for c in card if c != 'J')]
            if card.count('J') == 3:
                assert len(other) == 2
                best = max(other, key=lambda x: cardvalue(x))
                other = card.replace('J', best)
                return [value(other)[0]] + [cardvalue(card)]
            else:
                best = [c for c in other if card.count(c) == 3 and c != 'J']
                assert len(best) == 1
                other = card.replace('J', best[0])
                return [value(other)[0]] + [cardvalue(card)]
        if cardtype[0] == 60:
            other = [*set(c for c in card if c != 'J')]
            if card.count('J') == 2:
                best = [c for c in other if card.count(c) == 2 and c != 'J']
                assert len(best) == 1
                best = best[0]
                other = card.replace('J', best)
                return [value(other)[0]] + [cardvalue(card)]
            else:
                best = [c for c in other if card.count(c) == 2 and c != 'J']
                assert len(best) == 2
                best = max(best, key=lambda x: cardvalue(x))
                other = card.replace('J', best)
                return [value(other)[0]] + [cardvalue(card)]
        if cardtype[0] == 50:
            other_cards = [*set(c for c in card if c != 'J')]
            if card.count('J') == 2:
                best = [c for c in other_cards if card.count(c) == 1 and c != 'J']
                best = max(best, key=lambda x: cardvalue(x))
                other = card.replace('J', best)
                return [value(other)[0]] + [cardvalue(card)]
            else:
                best = [c for c in other_cards if card.count(c) == 2 and c != 'J']
                assert len(best) == 1
                other = card.replace('J', best[0])
                return [value(other)[0]] + [cardvalue(card)]
        other = [*set(c for c in card if c != 'J')]
        best = max(other, key=lambda x: cardvalue(x))
        other = card.replace('J', best)
        return [value(other)[0]] + [cardvalue(card)]
    if card == card[0] * 5:
        seen.add('royalflush')
        return [100, cardvalue(card)]
    if 4 in counter.values():
        seen.add('four')
        return [90, cardvalue(card)]
    if set(counter.values()) == {3, 2}:
        seen.add('full')
        return [80, cardvalue(card)]
    if set(counter.values()) == {3, 1}:
        seen.add('three')
        return [70, cardvalue(card)]
    if sorted(list(counter.values())) == [1, 2, 2]:
        seen.add('two')
        return [60, cardvalue(card)]
    if 2 in counter.values():
        seen.add('pair')
        return [50, cardvalue(card)]
    seen.add('high')
    return [0, cardvalue(card)]


for i, d in enumerate(data):
    data[i].append(value(d[0]))
    # data[i].append(value(d[0], True))  # Obviously ignore_joker was added for Part 2, but this replicates Part 1
# print(seen)

data = sorted(data, key=lambda x: x[-1])

for d in data[:5]:
    print(d)
# assert(len(seen) == 7)

out = 0
for i, (card, bid, rank) in enumerate(data):
    assert type(rank[0]) == int and type(rank[1]) == list and len(rank[1]) == 5
    out += int(bid) * (i + 1) 

print(f'out: {out}')

# 254909816

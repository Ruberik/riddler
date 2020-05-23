import collections

words = [x.strip() for x in open('words.txt')]
words = [x[1] for x in reversed(sorted([(len(word), word) for word in words]))]

states = [set(x.strip().lower()) for x in open('states.txt')]

def statecount(word):
  global states
  count = 0
  for state in states:
    for letter in word:
      if letter in state:
        count += 1
        break
  return count

def whichmackerel(word):
    if statecount(word) == 49:
        for state in states:
            for letter in word:
                if letter in state:
                    break
            else:
                return ''.join(sorted(state))

goodlen = 0
for word in words:
  if goodlen > len(word):
    break
  if statecount(word) == 49:
    print(word)
    goodlen = len(word)

print()

cnt = collections.defaultdict(int)

for word in words:
    which = whichmackerel(word)
    if which is not None:
        cnt[which] += 1
print(sorted([(v, k) for k, v in cnt.items()]))

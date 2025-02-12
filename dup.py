# with open("american-english2") as f:
#     s = f.read()
# print(len(s.split("\n")))
# s = set(s.split("\n"))
# print(len(s))
# # # filtered_lines = [line for line in s if len(line) == 2]
# filtered_lines = [line for line in s if line not in ["dds", "est", ""iii", "clxiv", "lxix", "clxix", "transfixt", "xix", "bewixt", "xcix", "lix", "xxxix", "xxix", "eh", "fr", "ax", "ah", "uh", "yo", "ow", "ex", "km", "kg", "ad", "ft", "aw", "gs", "mg", "xx", "re", "lo", "ed", "es", "pH", "nu", "ha", "wk", "fa", "cc", "oz", "yr", "pa", "ht", "mi", "pj", "ma", "fl", "cm", "mu", "ea", "um", "mm", "rs", "ms", "op", "kW", "ay", "dz", "mo", "hp", "ye", "pd", "wt", "gm", "cs", "ii", "hr", "ks", "xv", "ml", "gr", "pl", "dd", "iv", "fr" "xx", "rm", "lb", "cf", "pg", "ox", "ix", "vi", "xi", "ls", "dB", "pi", "ch", "ca", "la", "em", "ct", "sq", "yd", "sh", "ti", "ho", "pt", "qt", "ga", "cu", "ts", "pp", "kc"]]
# print(filtered_lines)
# print("\n".join(filtered_lines))
# new_f = open("american-english2", "w")
# new_f.write("\n".join(filtered_lines))


with open("american-english2") as f:
    s = f.read()
s = s.split("\n")
for string in range(len(s)):
    if len(s[string]) == 1:
        print(s[string], string)


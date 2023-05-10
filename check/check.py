with open("C:/Users/RODRIGO/Desktop/CESAR/VSCode C/Metrorec-Project/check/ids.txt", "r") as ids:
    linhas = ids.readlines()
# print(linhas)
cont = 0
ident = []
for linha in linhas:
    if linha[:2] == "Id":
        ident.append(linha[3:])
    if "chegando" in linha:
        cont+=1
    # else:
        # print(linha[:2])
# print(ident)

repetidos = []
for num in ident:
    qtd = ident.count(num)
    if qtd != 1:
        repetidos.append(num)

print(repetidos)
print(cont)
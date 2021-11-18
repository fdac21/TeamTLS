import pandas

df = pandas.read_csv('majestic_million.csv')

print(df)

df_com = df[df['TLD'] == 'com'].head(10000)

df_com['Domain'].to_csv('ten_thousand_com.txt', header=None, index=None)
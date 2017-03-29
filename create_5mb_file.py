f = open('test_data_file', 'w')
for i in range(0, 800000):
   f.write(str(i)+' ')

f.close()

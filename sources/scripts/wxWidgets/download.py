# -*- coding: utf-8 -*-

import requests
import re
from bs4 import BeautifulSoup
import os
import zipfile
import tempfile
import time


#-----------------------------------------------------------------------------------
# Формирует строку отображения для процентов
def StringForPercents(percents):
	result = ""
	num_cells = 40
	filled = percents / (100 / num_cells);
	for i in range(1, num_cells):
		if(i <= percents / (100 / num_cells)):
			result += '|'
		else:
			result += '-'
			
	result += ' ' + str(int(percents)) + '%'
	return result;


#-----------------------------------------------------------------------------------
def UnZipFile(file, path):
    print("Unzip file " + file)
    dir = '.'
    zf = zipfile.ZipFile(file)
    unzipped = 0
    for fn in zf.namelist():
        zf.extract(fn, path)
        unzipped += 1
        print("\r" + StringForPercents(unzipped / len(zf.namelist()) * 100) + " unzipped", end='')
    zf.close()


#-----------------------------------------------------------------------------------
def ExtractName(soup):
    return soup.find('div', {'class': 'row'}).\
                find('div', {'class': 'col-sm-6'}).\
                find('div', {'class': 'card my-3'}).\
                find('div', {'class': 'card-body'}).\
                find('a').get('href')


#-----------------------------------------------------------------------------------
def GetNameFile(url):
    
    # Здесь текст html-файла
    page = requests.get(url).text
    
    #with open('page.html', 'w') as out_file:
    #    out_file.write(page)
        
    soup = BeautifulSoup(page, "lxml")
    
    # Извлекаем название zip-файла
    file_name = ExtractName(soup)
    
    return file_name

#-----------------------------------------------------------------------------------    
def SpeedToString(speed):
    if(speed < 1e3):
        return str(float('{:.1f}'.format(speed))) + " b/s"
    elif(speed < 1e6):
        return str(float('{:.1f}'.format(speed / 1e3))) + " kb/s"
    return str(float('{:.1f}'.format(speed / 1e6))) + " Mb/s"

#-----------------------------------------------------------------------------------    
def GetFile(src, dest):

    # Сколько загружено байт
    loaded_size = 0
    
    # Сюда будем сохранять файл
    zip_file = open(dest, 'wb')
    
    # Даём запрос на чтение файла
    r = requests.get(src, stream = True)
    
    # Узнаём размер файла
    size_file = int(r.headers['content-length'])
    
    # Размер элементарной порции чтения
    size_chunk = int(size_file / 100.0)
    
    time_start = time.monotonic()
      
    for chunk in r.iter_content(chunk_size = size_chunk):
        zip_file.write(chunk)
        loaded_size += len(chunk)
        percents = StringForPercents(loaded_size / size_file * 100)
        speed =  loaded_size / (time.monotonic() - time_start)
        print("\r" + percents + " : " + str(int(loaded_size / 1024)) + " kb from " + str(int(size_file / 1024)) + " kb " + SpeedToString(speed), end='')
    
    zip_file.close()
    print()



#-----------------------------------------------------------------------------------
site = 'https://www.wxwidgets.org/'

url = site + 'downloads/'

full_name = GetNameFile(url)

# Создаём временный каталог для скачиваемых файлов, если ещё не создан
temp_dir = '..\\..\\ThirdParty\\wxWidgets\\'

if not os.path.exists(temp_dir):
	os.makedirs(temp_dir)

print('\nDownload file ' + full_name)

file = temp_dir + re.split('/', full_name)[-1]

GetFile(full_name, file)

UnZipFile(file, temp_dir)

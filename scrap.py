from bs4 import BeautifulSoup
import requests

url="https://modern.ircdocs.horse/"

# Make a GET request to fetch the raw HTML content
html_content = requests.get(url).text

# Parse the html content
soup = BeautifulSoup(html_content, "html.parser")
# print(soup.prettify())
# for el in soup.find_all("h3"):
flag = 0
for el in soup.findAll(['h3', 'div']):
	if (el in soup.find_all('h3') and el.get('id') == "rplbounce-010"):
		flag = 1
	if (flag == 1 and el in soup.find_all('h3')):
		print(el.get('id'))
	elif (flag == 1):
		if (hasattr(el.code, 'string')):
			print(el.code.string)
	elif (flag == -1):
		if (hasattr(el.code, 'string')):
			print(el.code.string)
		flag = 0
	if (el in soup.find_all('h3') and el.get('id') == "rplsaslmechs-908"):
		flag = -1
# for el in soup.findAll(['code']):
# 	print(el.string)
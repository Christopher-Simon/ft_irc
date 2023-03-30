from bs4 import BeautifulSoup
import requests

url="https://modern.ircdocs.horse/"

# Make a GET request to fetch the raw HTML content
html_content = requests.get(url).text

# Parse the html content
soup = BeautifulSoup(html_content, "html.parser")
# print(soup.prettify())
# for el in soup.find_all("h3"):
for el in soup.findAll(['h3', 'div']):
	if (el in soup.find_all('h3')):
		print(el.get('id'))
	else:
		print("YOLO:")
		print(el.code)
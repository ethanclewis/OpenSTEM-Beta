import urllib.request

def check_internet_connection():
    url = "http://www.google.com"
    try:
        response = urllib.request.urlopen(url, timeout=5)
        print("Internet is connected.")
    except urllib.error.URLError as e:
        print("No internet connection.")

if __name__ == "__main__":
    check_internet_connection()
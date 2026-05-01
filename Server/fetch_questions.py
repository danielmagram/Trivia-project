import sqlite3
import requests
import html

# 1. Connect directly to your C++ server's database file
db_path = 'triviaDB.sqlite' 
conn = sqlite3.connect(db_path)
cursor = conn.cursor()

# Make sure the table exists just in case the C++ server hasn't created it yet
cursor.execute('''
    CREATE TABLE IF NOT EXISTS QUESTIONS (
        QUESTION_ID INTEGER PRIMARY KEY AUTOINCREMENT,
        QUESTION TEXT NOT NULL,
        CORRECT_ANS TEXT NOT NULL,
        ANS2 TEXT NOT NULL,
        ANS3 TEXT NOT NULL,
        ANS4 TEXT NOT NULL
    )
''')

# 2. Fetch 10 random multiple-choice questions from the OpenTDB API
print("Fetching questions from OpenTDB...")
url = "https://opentdb.com/api.php?amount=10&difficulty=medium&type=multiple"
response = requests.get(url)
data = response.json()

# 3. Parse the JSON and insert into the database
if data['response_code'] == 0:
    
    print("Clearing old questions from the database...")
    cursor.execute('DELETE FROM QUESTIONS')
    cursor.execute("DELETE FROM sqlite_sequence WHERE name='QUESTIONS'")
    
    for item in data['results']:
        question = html.unescape(item['question'])
        correct = html.unescape(item['correct_answer'])
        incorrects = [html.unescape(ans) for ans in item['incorrect_answers']]
        
        cursor.execute('''
            INSERT INTO QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)
            VALUES (?, ?, ?, ?, ?)
        ''', (question, correct, incorrects[0], incorrects[1], incorrects[2]))
        
        print(f"Added: {question}")

    conn.commit()
    print("\nSuccessfully inserted 10 questions into the database!")
else:
    print("Failed to fetch questions from the API.")

conn.close()
import requests
import json
import sqlite3
import random
import html

url = "https://opentdb.com/api.php?amount=10&type=multiple"
response = requests.get(url)
data = json.loads(response.text)

connected = sqlite3.connect("Trivia_Shmuel_Ilya_2025/TriviaDB.db")
cursor = connected.cursor()

sqlStatement = "INSERT INTO questions(question,answer0,answer1,answer2,answer3,correct_answer_id) VALUES (?,?,?,?,?,?)"

if data["response_code"] == 0:
    for question_Data in data["results"]:
        correct_answer = html.unescape(question_Data["correct_answer"])
        answers = [html.unescape(ans) for ans in question_Data["incorrect_answers"]]
        answers.append(correct_answer)

        random.shuffle(answers)

        values = (
            html.unescape(question_Data["question"]),
            answers[0],
            answers[1],
            answers[2],
            answers[3],
            answers.index(correct_answer)
        )

        print(correct_answer)
        print(values)
        cursor.execute(sqlStatement, values)

connected.commit()
connected.close()

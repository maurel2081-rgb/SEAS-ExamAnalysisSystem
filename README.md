# SEAS — Smart Examination Analysis System

A C program that automatically grades MCQ exams and flags suspicious 
answer-pattern pairs for human investigation.

Built in 2 months at FCSS Yaoundé, Cameroon.

---

## What It Does

- Grades multiple-choice exams automatically from student input
- Runs a sequential, non-redundant pairwise comparison of answers 
  between every pair of students
- Computes an answer-similarity score per pair
- Analyzes seating coordinates to detect physically close students
- Applies a seat-proximity boost when a similar pair is seated nearby
- Produces a final **suspicion score** (0–100) per pair
- Flags pairs above a threshold for human invigilator investigation

> ⚠️ SEAS does **not** accuse students. It flags statistical patterns 
> for a human to review. Suspicion ≠ proof.

---

## Why It Exists

Manual MCQ grading is slow, and detecting collusion by eye is nearly 
impossible in large exam halls. SEAS automates both: it grades in 
seconds and highlights pairs whose answers match more than chance 
would explain, weighted by how close they sat.

---

## How It Works (Pipeline)

1. **Input** — marking guide, student names, seat coordinates, answers
2. **Grading** — each student's answers compared against the guide
3. **Pairwise comparison** — every student compared to every other 
   student exactly once (`n(n-1)/2` pairs)
4. **Similarity** — matched answers ÷ total questions
5. **Proximity check** — are the two students seated close?
6. **Scoring** — similarity score + proximity boost → suspicion score
7. **Flagging** — pairs above the threshold are reported

---

## Build & Run

### Requirements
- A C compiler (`gcc` or `clang`)
- Windows for color output (works on Linux/Mac without color)

### Compile
```bash
gcc "SEAS UPDATED.c" -o seas
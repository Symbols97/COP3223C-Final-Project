/*
 * main.c
 *
 *  Created on: Dec 2, 2022
 *      Author: ianragan
 */
typedef struct Student {

	int id;
	char *first_name;
	char *last_name;
	int day;
	int month;
	int year;

} Student;

typedef struct Course {
	char *id;
	char *course_name;
	float total_credit;
} Course;

typedef struct Enrollment {

	int student_id;
	char *course_id;
	char *semester;
	float score;

} Enrollment;

Student* loadStudents(int *numStudents);
Course* loadCourses(int *numCourses);
Enrollment* loadEnrollment(int *numEnrollments);

void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
void printResult(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int index, int code);
void printSearchStudent(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int id);
void printSearchCourse(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int id);
void sort(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
char calculateGrade(float n);
void freeMemory(Student *studentsList, int total_students, Course *courseList, int total_courses, Enrollment *enrollmentList, int total_enrollment, int code);

#define maxLength 51

char studentfile[31];
char coursefile[31];
char enrollmentfile[31];

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leak_detector_c.h"

int main() {

	atexit(report_mem_leak);

	scanf("%s", studentfile);
	scanf("%s", coursefile);
	scanf("%s", enrollmentfile);

	Student *studentList;
	int total_students;
	studentList = loadStudents(&total_students);

	Course *courseList;
	int total_courses;
	courseList = loadCourses(&total_courses);

	Enrollment *enrollmentList;
	int total_enrollment;
	enrollmentList = loadEnrollment(&total_enrollment);

	menu(studentList, total_students, courseList, total_courses, enrollmentList, total_enrollment);

}

//Load the student file
Student* loadStudents(int *numStudents) {

	FILE *student_info = fopen(studentfile, "r");

	int id;
	char fName[maxLength];
	char lName[maxLength];
	int day;
	int month;
	int year;

	fscanf(student_info, "%d", numStudents);

	Student *ArrayofStudents = (Student*) malloc(*numStudents * sizeof(Student));

	for (int i = 0; i < *numStudents; i++) {
		fscanf(student_info, "%d %s %s %d %d %d", &id, fName, lName, &day, &month, &year);

		ArrayofStudents[i].id = id;

		int fNameLength = strlen(fName) + 1;
		ArrayofStudents[i].first_name = (char*) malloc(fNameLength * sizeof(char));
		strcpy(ArrayofStudents[i].first_name, fName);

		int lNameLength = strlen(lName) + 1;
		ArrayofStudents[i].last_name = (char*) malloc(lNameLength * sizeof(char));
		strcpy(ArrayofStudents[i].last_name, lName);

		ArrayofStudents[i].day = day;
		ArrayofStudents[i].month = month;
		ArrayofStudents[i].year = year;
	}
	fclose(student_info);
	return ArrayofStudents;

}

//Load the course file
Course* loadCourses(int *numCourses) {

	FILE *course_info = fopen(coursefile, "r");

	char id[maxLength];
	char name[maxLength];
	float credit;

	fscanf(course_info, "%d", numCourses);

	Course *ArrayofCourses = (Course*) malloc(*numCourses * sizeof(Course));

	for (int i = 0; i < *numCourses; i++) {

		fscanf(course_info, "%s %s %f", id, name, &credit);

		int idLength = strlen(id) + 1;
		ArrayofCourses[i].id = (char*) malloc(idLength * sizeof(char));
		strcpy(ArrayofCourses[i].id, id);

		int nameLength = strlen(name) + 1;
		ArrayofCourses[i].course_name = (char*) malloc(nameLength * sizeof(char));
		strcpy(ArrayofCourses[i].course_name, name);

		ArrayofCourses[i].total_credit = credit;
	}
	fclose(course_info);
	return ArrayofCourses;
}

//Load the enrollment file
Enrollment* loadEnrollment(int *numEnrollment) {

	FILE *enrollment_info = fopen(enrollmentfile, "r");

	int student_id;
	char course_id[maxLength];
	char semester[maxLength];
	float score;

	fscanf(enrollment_info, "%d", numEnrollment);

	Enrollment *ArrayofEnrollment = (Enrollment*) malloc(*numEnrollment * sizeof(Enrollment));

	for (int i = 0; i < *numEnrollment; i++) {

		fscanf(enrollment_info, "%d %s %s %f", &student_id, course_id, semester, &score);
		ArrayofEnrollment[i].student_id = student_id;

		int courseIDLength = strlen(course_id) + 1;
		ArrayofEnrollment[i].course_id = (char*) malloc(courseIDLength * sizeof(char));
		strcpy(ArrayofEnrollment[i].course_id, course_id);

		int semesterLength = strlen(semester) + 1;
		ArrayofEnrollment[i].semester = (char*) malloc(semesterLength * sizeof(char));
		strcpy(ArrayofEnrollment[i].semester, semester);

		ArrayofEnrollment[i].score = score;
	}
	fclose(enrollment_info);
	return ArrayofEnrollment;

}

//Menu for all of the commands
void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls) {

	while (1) {
		char command[maxLength];
		char option[maxLength];
		char string_input[maxLength];
		int num_input;
		char semester[maxLength];
		int code = 0;
		int match = 0;
		printf("==================\n");
		scanf("%s", command);

		if (strcmp(command, "search_students") == 0) { //Checks if the command is the search_student command. Then checks the option
			code = 1;
			scanf("%s", option);
			if (strcmp(option, "lname") == 0) { // First Option LAST NAME
				scanf("%s", string_input);
				printf("------------------\n");
				for (int i = 0; i < amountStudents; i++) {
					if (strcmp(string_input, ArrayOfStudents[i].last_name) == 0) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
						printf("------------------\n");
					}
				}
				if (match == 0) {
					printf("not found\n");
					printf("------------------\n");
				}

			} else if (strcmp(option, "fname") == 0) { // First Option FIRST NAME
				scanf("%s", string_input);
				printf("------------------\n");
				for (int i = 0; i < amountStudents; i++) {
					if (strcmp(string_input, ArrayOfStudents[i].first_name) == 0) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
						printf("------------------\n");
					}
				}
				if (match == 0) {
					printf("not found\n");
					printf("------------------\n");
				}

			} else if (strcmp(option, "id") == 0) { // Second Option ID
				scanf("%d", &num_input);
				printf("------------------\n");
				for (int i = 0; i < amountStudents; i++) {
					if (num_input == ArrayOfStudents[i].id) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
						printf("------------------\n");
					}
				}
				if (match == 0) {
					printf("not found\n");
					printf("------------------\n");
				}
			} else if (strcmp(option, "byear") == 0) { // Third Option BY YEAR
				scanf("%d", &num_input);
				printf("------------------\n");
				for (int i = 0; i < amountStudents; i++) {
					if (num_input == ArrayOfStudents[i].year) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
						printf("------------------\n");
					}
				}
				if (match == 0) {
					printf("not found\n");
					printf("------------------\n");
				}

			}
		} // End of search_student command

		if (strcmp(command, "search_course") == 0) {
			scanf("%s", option);
			if (strcmp(option, "cid") == 0) {
				scanf("%s", string_input);
				printf("------------------\n");
				code = 2;

				for (int i = 0; i < amountCourses; i++) {
					if (strcmp(string_input, ArrayOfCourses[i].id) == 0) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
						printf("------------------\n");
					}

				}

				if (match == 0) {
					printf("not found\n");
					printf("------------------\n");
				}

			} else if (strcmp(option, "cid_semester") == 0) {
				scanf("%s", string_input);
				scanf("%s", semester);
				printf("------------------\n");
				code = 3;
				for (int i = 0; i < amountCourses; i++) {
					if (strcmp(string_input, ArrayOfCourses[i].id) == 0) {
						match = 1;
						printResult(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, i, code);
					}
				}
				for (int i = 0; i < amountEnrolls; i++) {
					int cindex = 0;
					int index = 0;
					if ((strcmp(semester, ArrayOfEnrollments[i].semester) == 0) && (strcmp(string_input, ArrayOfEnrollments[i].course_id) == 0)) {
						cindex = i;
						index++;
						for (int k = 0; k < amountEnrolls; k++) {
							if (ArrayOfStudents[k].id == ArrayOfEnrollments[cindex].student_id) {
								char grade = calculateGrade(ArrayOfEnrollments[cindex].score);
								printf("%s %s %s %.0f %c\n", ArrayOfStudents[k].first_name, ArrayOfStudents[k].last_name, ArrayOfEnrollments[cindex].semester, ArrayOfEnrollments[cindex].score, grade);
							}
						}
					}
				}

				if (match == 0) {
					printf("not found\n");
				}
				printf("------------------\n");
			}
		}

		if (strcmp(command, "add_course") == 0) {
			int num = 0;
			int add = 0;
			scanf("%s %s %d", option, string_input, &num_input);

			for (int i = 0; i < amountCourses; i++) {
				if (strcmp(option, ArrayOfCourses[i].id) == 0) {
					printf("------------------\n");
					printf("violation of course id uniqueness. Cannot add the record.\n");
					printf("------------------\n");
					add = 1;
				}
			}

			if (add == 0) {
				FILE *course_info = fopen(coursefile, "r+");
				fscanf(course_info, "%d", &num);

				num++;
				fseek(course_info, 0, SEEK_SET);
				fprintf(course_info, "%d\n", num);
				fseek(course_info, 0, SEEK_END);
				fprintf(course_info, "\n%s %s %d", option, string_input, num_input);
				fclose(course_info);

				freeMemory(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, 2);

				ArrayOfCourses = loadCourses(&amountCourses);
				printf("------------------\n");
				printf("record added\n");
				printf("------------------\n");

			}

		}

		// Count Command
		if (strcmp(command, "count") == 0) {
			scanf("%s", option);

			if (strcmp(option, "students") == 0) { // Prints the total student count

				printf("------------------\n");
				printf("total students %d\n", amountStudents);
				printf("------------------\n");

			} else if (strcmp(option, "students_course") == 0) { // Prints each course ID and the amount of students taking that course

				printf("------------------\n");
				int counter;
				for (int i = 0; i < amountCourses; i++) {
					counter = 0;
					for (int j = 0; j < amountEnrolls; j++) {
						if (strcmp(ArrayOfCourses[i].id, ArrayOfEnrollments[j].course_id) == 0) {
							counter++;
						}
					}

					printf("%s %d\n", ArrayOfCourses[i].id, counter);
				}

				printf("------------------\n");

			} else if (strcmp(option, "students_semester") == 0) { // Prints all the semesters and the amount of students taking courses in that semester
				int length = 0;
				int exists = 0;
				char completed_list[amountEnrolls][maxLength];
				int student_count[amountStudents];

				//Sets all the indexes to 0
				for (int k = 0; k < amountStudents; k++) {
					student_count[k] = 0;
				}

				for (int i = 0; i < amountEnrolls; i++) {
					for (int j = 0; j < length; j++) {
						exists = 0;
						if (strcmp(completed_list[j], ArrayOfEnrollments[i].semester) == 0) {
							student_count[j]++;
							exists = 1;
							break;
						}
					}

					if (exists == 0) {
						strcpy(completed_list[length], ArrayOfEnrollments[i].semester);
						student_count[length]++;
						length++;
					}
				}
				printf("------------------\n");
				for (int i = 0; i < length; i++) {
					printf("%s %d\n", completed_list[i], student_count[i]);
				}
				printf("------------------\n");

			}
		}

		if (strcmp(command, "sort") == 0) { //Sort Command
			printf("------------------\n");
			sort(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
			printf("------------------\n");
		} // End of Sort Command

		if (strcmp(command, "exit") == 0) { // Exit Command
			printf("------------------\n");
			printf("Bye!");
			freeMemory(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, 0);
			break;
		}
	}
}

//Sort command
void sort(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls) {
	int arr[maxLength];
	for (int i = 0; i < amountStudents; i++) {
		arr[i] = ArrayOfStudents[i].id;
	}
	for (int i = 0; i < amountStudents; i++) {
		for (int j = i + 1; j < amountStudents; j++) {
			if (arr[i] > arr[j]) {
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
	for (int i = 0; i < amountStudents; i++) {
		for (int j = 0; j < amountStudents; j++) {
			if (arr[i] == ArrayOfStudents[j].id) {
				int index = j;
				printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[index].id, ArrayOfStudents[index].first_name, ArrayOfStudents[index].last_name, ArrayOfStudents[index].month, ArrayOfStudents[index].day, ArrayOfStudents[index].year);
			}
		}
	}
}

//Since the search functions need relatively similar info this calls the specific search based on a code I assigned
void printResult(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int id, int code) {

	switch (code) {
	case 1:
		printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[id].id, ArrayOfStudents[id].first_name, ArrayOfStudents[id].last_name, ArrayOfStudents[id].month, ArrayOfStudents[id].day, ArrayOfStudents[id].year);
		printSearchStudent(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, id);
		break;
	case 2:
		printf("%s %.0f\n", ArrayOfCourses[id].course_name, ArrayOfCourses[id].total_credit);
		printSearchCourse(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls, id);
		break;
	case 3:
		printf("%s %.0f\n", ArrayOfCourses[id].course_name, ArrayOfCourses[id].total_credit);
		break;
	}
}

//Prints all the Search_Student command
void printSearchStudent(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int id) {

	int index = 0;
	int cindex = 0;
	char grade;
	for (int i = 0; i < amountEnrolls; i++) {
		if (ArrayOfStudents[id].id == ArrayOfEnrollments[i].student_id) {
			index = i;
			for (int j = 0; j < amountCourses; j++) {
				if (strcmp(ArrayOfEnrollments[i].course_id, ArrayOfCourses[j].id) == 0) {
					cindex = j;
				}
			}
			grade = calculateGrade(ArrayOfEnrollments[index].score);
			printf("%s %s %s %.0f %c\n", ArrayOfEnrollments[index].course_id, ArrayOfCourses[cindex].course_name, ArrayOfEnrollments[index].semester, ArrayOfEnrollments[index].score, grade);
		}
	}
}

//Print for searching course by CID
void printSearchCourse(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, int id) {

	int index = 0;
	int cindex = 0;
	char grade;

	for (int i = 0; i < amountEnrolls; i++) {
		if (strcmp(ArrayOfCourses[id].id, ArrayOfEnrollments[i].course_id) == 0) {
			index = i;
			for (int j = 0; j < amountStudents; j++) {
				if (ArrayOfEnrollments[i].student_id == ArrayOfStudents[j].id) {
					cindex = j;
				}
			}
			grade = calculateGrade(ArrayOfEnrollments[index].score);
			printf("%s %s %s %.0f %c\n", ArrayOfStudents[cindex].first_name, ArrayOfStudents[cindex].last_name, ArrayOfEnrollments[index].semester, ArrayOfEnrollments[index].score, grade);
		}
	}
}

//Converts number grade to letter grade
char calculateGrade(float grade) {
	char c;
	if (grade >= 90) {
		c = 'A';
	} else if (grade >= 80 && grade <= 89.99) {
		c = 'B';
	} else if (grade >= 70 && grade <= 79.99) {
		c = 'C';
	} else if (grade >= 60 && grade <= 69.99) {
		c = 'D';
	} else {
		c = 'F';
	}
	return c;
}

//Frees all the memory. Switch statement for the add_course so i can free just one specific file depending on the use case
void freeMemory(Student *studentList, int total_students, Course *courseList, int total_courses, Enrollment *enrollmentList, int total_enrollment, int code) {

	switch (code) {
	//Free ALL at ONCE
	case 0:
		//Free Students
		for (int i = 0; i < total_students; i++) {
			free(studentList[i].first_name);
			free(studentList[i].last_name);
		}
		free(studentList);

		//Free Courses
		for (int i = 0; i < total_courses; i++) {
			free(courseList[i].course_name);
			free(courseList[i].id);
		}
		free(courseList);

		//Free Enrollments
		for (int i = 0; i < total_enrollment; i++) {
			free(enrollmentList[i].course_id);
			free(enrollmentList[i].semester);
		}
		free(enrollmentList);
		break;

		//Individually free each one if need be (mainly used for adding a course)
	case 1:
		for (int i = 0; i < total_students; i++) {
			free(studentList[i].first_name);
			free(studentList[i].last_name);
		}
		free(studentList);
		break;
	case 2:
		for (int i = 0; i < total_courses; i++) {
			free(courseList[i].course_name);
			free(courseList[i].id);
		}
		free(courseList);
		break;
	case 3:
		for (int i = 0; i < total_enrollment; i++) {
			free(enrollmentList[i].course_id);
			free(enrollmentList[i].semester);
		}
		free(enrollmentList);
		break;
	}
}

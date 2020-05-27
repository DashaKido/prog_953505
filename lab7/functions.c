#include "functions.h"
int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int get_int()
{
	int num = 0;
	char* input = get_string(stdin);
	if (length_of(input) >= 10) input[9] = '\0';
	int i = 0;
	for (; input[i] && input[i] >= '0' && input[i] <= '9'; i++)
		num = num * 10 + input[i] - '0';
	free(input);
	return num;
}
//-----------
int is_a_leap_year(int year)
{
	if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) return 0;
	return 1;
}
user* get_user_by_id(id some_id)
{
	box* whatever = get_item_by_id(some_id);
	user* output;
	if (whatever->g != NULL) output = whatever->g->image;
	else output = whatever->u;
	free(whatever);
	return output;
}
group* get_group_by_id(id some_id)
{
	box* whatever = get_item_by_id(some_id);
	group* output = whatever->g;
	free(whatever);
	return output;
}
int is_name_unique(int namespace, char* name)
{
	int i = 0;
	if (namespace == 0)
	{
		for (; i < num_of_groups; i++)
			if (are_strs_same(groups[i]->image->name, name)) return 0;
	}
	else if (namespace == 1)
	{
		for (; i < num_of_releases; i++)
			if (are_strs_same(releases[i]->name, name)) return 0;
	}
	else
	{
		for (; i < num_of_users; i++)
			if (are_strs_same(users[i]->name, name)) return 0;
	}
	return 1;
}
void display_song(song one)
{
	printf("����� ");
	out_str_wo_nl(one.name);
	printf(":\n");
	if (one.lyrics->num_of_lines != 0)
	{
		printf("�����:\n");
		int i = 0;
		for (; i < one.lyrics->num_of_lines; i++)
			printf("\t%s", one.lyrics->lines[i]);
	}
}
void display_release(id release_id)
{
	box* whatever = get_item_by_id(release_id);
	if (whatever->r == NULL) return;
	release* one = whatever->r;
	free(whatever);
	printf("---------------------------------\n");
	printf("%s", (one->type == 0 ? "Composition " : one->type == 1 ? "Single " :
		one->type == 2 ? "LP " : one->type == 3 ? "EP " : "Mixtape "));
	out_str_wo_nl(one->name);
	printf(" �� ");
	box* tmp = get_item_by_id(one->authors[0]);
	if (tmp->u != NULL) out_str_wo_nl(tmp->u->name);
	else out_str_wo_nl(tmp->g->image->name);
	if (one->num_of_authors > 1)
	{
		int i = 1;
		printf("\n������: ");
		for (; i < one->num_of_authors; i++)
		{
			box* tmp = get_item_by_id(one->authors[i]);
			if (tmp->u != NULL) out_str_wo_nl(tmp->u->name);
			printf(", ");
			free(tmp);
		}
		printf("\b\b.");
	}
	else printf(".");
	free(tmp);
	printf("\n---------------------------------\n");
	int i = 0;
	for (; i < one->num_of_songs; i++)
	{
		display_song(one->songs[i]);
		printf("\n---------------------------------\n");
	}
}
//------------------
void main_menu()
{
	while (1)
	{
		char chinput = 0;
		do
		{
			system("cls");
			printf("�������:\t ���:\n\n");
			printf(" 1\t\t �����������.\n");
			printf(" 2\t\t ����.\n");
			printf(" 0\t\t �����.\n");
			chinput = _getch();
		} while (chinput != '0' && chinput != '1' && chinput != '2');
		if (chinput == '0')	break;
		if (chinput == '1') sign_up();
		else sign_in();
	}
}
void sign_up()
{
	system("cls");
	char user_or_group;
	do
	{
		printf("�� ������������/������(a) ��� ������(g)? \n");
		user_or_group = _getch();
	} while (user_or_group != 'a' && user_or_group != 'g');
	int num_of_members = 0;
	id* members = NULL;
	if (user_or_group == 'g')
	{
		char when;
		do
		{
			printf("\n�� ������ �������� ���������� �����(r) ��� �����(l)? \n");
			when = _getch();
		} while (when != 'r' && when != 'l');
		if (when == 'r')
		{
			int chinput;
			do
			{
				id new_member = select_one(1, 0, 0);
				if (new_member == -1)
				{
					printf("����� �� ��� ������. ");
				}
				else if (is_among(members, num_of_members, new_member))
				{
					printf("���� ������������ ��� ��������� � ������.");
				}
				else
				{
					members = (id*)realloc(members, sizeof(id) * (num_of_members + 1));
					if (members == NULL) report_merror();
					members[num_of_members++] = new_member;
					printf("���� ������������ ��� �������� � ������.");
				}
				printf("������� (n) ���� ����� ����������. ");
				printf("Press n if it\'s enough: ");
				chinput = _getch();
			} while (chinput != 'n');
		}
	}
	char* name;
	printf("\n������� ");
	if (user_or_group == 'a') printf("���� ���: ");
	else printf("�������� ������: ");
	name = get_string(stdin);
	while (!is_name_unique(2, name))
	{
		free(name);
		printf("\n��� ���� ������������ � ����� ������. ���������� �����... ");
		name = get_string(stdin);
	}
	if (user_or_group == 'a')
	{
		user* new_one = new_user(name);
		users = (user**)realloc(users, sizeof(user*) * (num_of_users + 1));
		if (users == NULL) report_merror();
		users[num_of_users++] = new_one;
		session(new_one->user_id);
	}
	else
	{
		group* new_one = new_group(name, num_of_members, members);
		groups = (group**)realloc(groups, sizeof(group*) * (num_of_groups + 1));
		if (groups == NULL) report_merror();
		groups[num_of_groups++] = new_one;
		session(new_one->image->user_id);
	}
}
void sign_in()
{
	system("cls");
	id you = select_one(1, 1, 0);
	if (you != -1) session(you);
}
id select_one(int among_users, int among_groups, int among_releases)
{
	if (among_users == 0 && among_groups == 0 && among_releases == 0) return -1;
	int num_of_options = (among_users ? num_of_users : 0) + (among_groups ? num_of_groups : 0) + (among_releases ? num_of_releases : 0);
	id* options = (id*)malloc(sizeof(id) * num_of_options);
	int i = 0, j;
	if (among_users)
	{
		for (j = 0; j < num_of_users; i++, j++)
			options[i] = users[j]->user_id;
	}
	if (among_groups)
	{
		for (j = 0; j < num_of_groups; i++, j++)
			options[i] = groups[j]->image->user_id;
	}
	if (among_releases)
	{
		for (j = 0; j < num_of_releases; i++, j++)
			options[i] = releases[j]->release_id;
	}
	char* tag = NULL;
	int num_of_available = num_of_options;
	id* available = filtered(options, &num_of_options, tag);
	while (1)
	{
		printf("\n�������:\t\t��� ������\n\n 0\t\t �����\n");
		for (i = 0; i < num_of_available; i++)
		{
			box* tmp = get_item_by_id(available[i]);
			int index = tmp->g != NULL ? 0 :
				tmp->r != NULL ? 1 : tmp->u->name != NULL ? 2 : -1;
			printf(" %d\t\t %s: ", i + 1, index == 0 ? "������" :
				index == 1 ? "����������" : index == 2 ? "������������" : NULL);
			out_str_wo_nl(tmp->g != NULL ? tmp->g->image->name : tmp->r != NULL ? tmp->r->name :
				tmp->u->name != NULL ? tmp->u->name : NULL);
			printf("\n");
			free(tmp);
		}
		printf("\n������� �� (ENTER) ��� ������: ");
		free(tag);
		tag = get_string(stdin);
		if (length_of(tag) == 2)
		{
			printf("\n������� ����� �����: ");
			int chinput, option = 0;
			while ((chinput = fgetc(stdin)) >= '0' && chinput <= '9')
				option = option * 10 + chinput - '0';
			if (option == 0) return -1;
			if (option <= num_of_available) return available[option - 1];
		}
		else
		{
			tag[length_of(tag) - 2] = '\0';
			free(available);
			num_of_available = num_of_options;
			available = filtered(options, &num_of_available, tag);
		}
	}
}
box* get_item_by_id(id tag)
{
	box* output = new_box();
	int i;
	for (i = 0; i < num_of_users; i++)
	{
		if (users[i]->user_id == tag)
		{
			output->u = users[i];
			return output;
		}
	}
	for (i = 0; i < num_of_groups; i++)
	{
		if (groups[i]->image->user_id == tag)
		{
			output->g = groups[i];
			return output;
		}
	}
	for (i = 0; i < num_of_releases; i++)
	{
		if (releases[i]->release_id == tag)
		{
			output->r = releases[i];
			return output;
		}
	}
	return output;
}
id* filtered(id* original, int* num_of_options, char* tag)
{
	id* output;
	size_t size = 0;
	size_t capacity = DEF_STRUCT_CAP;
	output = (id*)malloc(sizeof(id) * capacity);
	if (output == NULL) report_merror();
	int i = 0;
	for (; i < *num_of_options; i++)
	{
		box* check = get_item_by_id(original[i]);
		if (substr(tag, (check->u != NULL ? check->u->name : check->g != NULL ? check->g->image->name :
			check->r != NULL ? check->r->name : NULL)) != -1)
		{
			output[size++] = original[i];
		}
		free(check);
		if (size == capacity)
		{
			output = (id*)realloc(output, sizeof(id) * (capacity *= 2));
			if (output == NULL)	report_merror();
		}
	}
	output = (id*)realloc(output, sizeof(id) * size);
	*num_of_options = size;
	if (output == NULL && *num_of_options != 0)	report_merror();
	return output;
}
void session(id you)
{
	user* user_you = get_user_by_id(you);
	group* group_you = get_group_by_id(you);
	if (user_you == NULL) return;
	int chinput;
	do
	{
		system("cls");
		out_str_wo_nl(user_you->name);
		printf("\n\t������ �����: %d$,\n\t�����: %d$.\n\n",
			user_you->full_account, user_you->revenue);
		if (group_you != NULL)
		{
			printf("\t���������: ");
			if (group_you->num_of_members == 0)	printf("�����.\n\n");
			else
			{
				int i = 0;
				for (; i < group_you->num_of_members; i++)
				{
					user* member = get_user_by_id(group_you->members[i]);
					if (member != NULL)
					{
						out_str_wo_nl(member->name);
						printf(", ");
					}
				}
				printf("\b\b.\n\n");
			}
		}
		printf("�������:\t ���:\n"
			" 1\t\t �������� ������\n"
			" 2\t\t ��������� ���������\n"
			" 3\t\t ������ ���������\n"
			" 4\t\t ������� ��� ���������\n");
		if (group_you != NULL)
		{
			printf(" 5 \t\t �������� ���������\n"
				" 6\t\t ������� ���������\n");
		}
		printf(" 0\t\t �����\n");
		chinput = _getch();
		if (chinput == '1') //�������� ������
		{
			system("cls");
			printf("������� �����: ");
			add_money(you);
		}
		else if (chinput == '2') //��������� ���������
		{
			release_a_record(you);
		}
		else if (chinput == '3') //������ ���������
		{
			buy_a_record(you);
		}
		else if (chinput == '4') //������� ��� ���������
		{
			int i = 0;
			for (; i < user_you->num_of_purchased_releases; i++)
			{
				display_release(user_you->purchased_releases[i]);
				printf("\n**************************************\n");
			}
			printf("\n������� ����� �������, ����� ��������� � ���� �������...");
			_getch();
		}
		else if (chinput == '5' && group_you != NULL) //�������� ����������
		{
			id new_member = select_one(1, 0, 0);
			if (is_among(group_you->members, group_you->num_of_members, new_member))
			{
				printf("���� ������������ ��� ��������� � ������.");
				printf("������� ����� �������, ����� ��������� � ���� �������...");
				_getch();
				continue;
			}
			group_you->members = (id*)realloc(group_you->members, sizeof(id) * group_you->num_of_members + 1);
			if (group_you->members == NULL) report_merror();
			group_you->members[group_you->num_of_members++] = new_member;
			printf("���� ������������ ��� �������� � ������.");
			printf("������� ����� �������, ����� ��������� � ���� �������...");
			_getch();

		}
		else if (chinput == '6' && group_you != NULL) //������� ���������
		{
			if (group_you->num_of_members == 0)
			{
				printf("����� ������ �������.");
				printf("������� ����� �������, ����� ��������� � ���� �������...");
				_getch();
				continue;
			}
			printf("���� �� �� ������ ������?\n�������:\t\tT��� ��������:\n\n");
			printf(" 0\t\t ������\n");
			int i = 0;
			for (; i < group_you->num_of_members; i++)
			{
				printf(" %d\t\t ", i + 1);
				user* cur_member = get_user_by_id(group_you->members[i]);
				if (cur_member != NULL)	out_str_wo_nl(cur_member->name);
				printf("\n");
			}
			int choice;
			do
			{
				choice = get_int();
			} while (choice < 0 || choice > group_you->num_of_members);
			if (choice == 0) continue;
			if (group_you->num_of_members == 1)
			{
				free(group_you->members);
				group_you->members = NULL;
				group_you->num_of_members = 0;
			}
			for (; choice < group_you->num_of_members - 1; choice++)
				group_you->members[choice - 1] = group_you->members[choice];
			group_you->members = (id*)realloc(group_you->members, --group_you->num_of_members * sizeof(id));
			if (group_you->members == NULL)	report_merror();
			printf("���� ������������ ��� ������ �� ������.");
			printf("������� ����� �������, ����� ��������� � ���� �������...");
			_getch();
		}
	} while (chinput != '0');
}
void add_money(id you)
{
	user* user_you = get_user_by_id(you);
	if (user_you == NULL) return;
	int sum = get_int();
	if (sum < 0 || MAX_INT - user_you->full_account < sum)
	{
		printf("\n���������� �������� ��� ����� �����.");
		printf("\n������� ����� �������, ����� ��������� � ���� �������...");
		_getch();
		return;
	}
	user_you->full_account += sum;
	printf("\n%d$ ���� ��������� �� ��� �������.", sum);
	printf("\n������� ����� �������, ����� ��������� � ���� �������...");
	_getch();
	return;
}
void release_a_record(id you)
{
	system("cls");
	user* user_you = get_user_by_id(you);
	group* group_you = get_group_by_id(you);
	if (user_you == NULL) return;
	release* new_one = new_release();
	printf("��� �����...\n"
		"Composition(1)\tSingle(2)\tLP(3)\tEP(4)\tMixtape(5)?");
	do
	{
		new_one->type = _getch() - '1';
	} while (new_one->type < 0 || new_one->type > 4);
	printf("\n������� �������� ���������: ");
	char* tmp = get_string(stdin);
	if (length_of(tmp) == 2)
	{
		free(tmp);
		tmp = "��� ��������\n";
	}
	new_one->name = tmp;
	new_one->num_of_authors = 1 + (group_you != NULL ? group_you->num_of_members : 0);
	new_one->authors = (id*)malloc(sizeof(id) * new_one->num_of_authors);
	new_one->authors[0] = you;
	int i = 1;
	for (; i < new_one->num_of_authors; i++)
	{
		new_one->authors[i] = group_you->members[i - 1];
	}
	user_you->purchased_releases = (id*)realloc(user_you->purchased_releases,
		user_you->num_of_purchased_releases + 1);
	if (user_you->purchased_releases == NULL) report_merror();
	user_you->purchased_releases[user_you->num_of_purchased_releases++] =
		new_one->release_id;
	do
	{
		printf("\n������� ���� �������: ");
		new_one->dropped = set_date();
	} while (new_one->dropped == NULL);
	do
	{
		printf("\n������� ���������� ����� �� ���������: ");
		new_one->num_of_songs = get_int();
	} while (new_one->num_of_songs <= 0 || new_one->num_of_songs > SONGS_ON_THE_RECORD_UPPER_LIMIT);
	new_one->songs = (song*)malloc(sizeof(song) * new_one->num_of_songs);
	i = 0;
	for (; i < new_one->num_of_songs; i++)
	{
		printf("\n���������� ����� #%d:\n", i + 1);
		new_one->songs[i] = set_song();
	}
	do
	{
		printf("\n������� ���� ���������: ");
		new_one->price = get_int();
	} while (new_one->price <= 0);
	user_you->releases = (id*)realloc(user_you->releases, sizeof(id) * (user_you->num_of_releases + 1));
	if (user_you->releases == NULL)	report_merror();
	user_you->releases[user_you->num_of_releases++] = new_one->release_id;
	releases = (release**)realloc(releases, sizeof(release*) * (num_of_releases + 1));
	if (releases == NULL) report_merror();
	releases[num_of_releases++] = new_one;
}
void buy_a_record(id you)
{
	box* whatever = get_item_by_id(you);
	user* user_you = NULL;
	if (whatever->u != NULL) user_you = whatever->u;
	else if (whatever->g != NULL) user_you = whatever->g->image;
	else return;
	free(whatever);
	id record_id = select_one(0, 0, 1);
	if (record_id == -1) return;
	whatever = get_item_by_id(record_id);
	release* selected;
	if ((selected = whatever->r) == NULL)
	{
		free(whatever);
		return;
	}
	free(whatever);
	if (is_among(user_you->purchased_releases, user_you->num_of_purchased_releases,
		selected->release_id))
	{
		printf("��� ��� � ��� ����.");
		printf("\n������� ����� �������, ����� ��������� � ���� �������...");
		_getch();
		return;
	}
	printf("\n%s�� ������������� ������ �� ������ �� %d$? (y / n) ", selected->name, selected->price);
	int chinput;
	do
	{
		chinput = _getch();
	} while (chinput != 'y' && chinput != 'n');
	if (chinput == 'y')
	{
		if (user_you->full_account < selected->price)
		{
			printf("\n� ��� ������������ �������.");
			printf("\n������� ����� �������, ����� ��������� � ���� �������...");
			_getch();
			return;
		}
		else
		{
			user_you->full_account -= selected->price;
			box* author = get_item_by_id(selected->authors[0]);
			user* user_author = NULL;
			if (author->u != NULL) user_author = author->u;
			else if (author->g != NULL) user_author = author->g->image;
			else return;
			free(author);
			if (selected->price <= MAX_INT - user_author->revenue)
				user_author->revenue += selected->price;
			if (selected->price <= MAX_INT - user_author->full_account)
				user_author->full_account += selected->price;
			user_you->purchased_releases = (id*)realloc(user_you->purchased_releases,
				sizeof(id) * (user_you->num_of_purchased_releases + 1));
			user_you->purchased_releases[user_you->num_of_purchased_releases++] = record_id;
			printf("\n�� ������ ��.");
			printf("\n������� ����� �������, ����� ��������� � ���� �������...");
			_getch();
			return;
		}
	}
}
date* set_date()
{
	char* str = get_string(stdin);
	int i = 0, m = 0, d = 0, y = 0;
	for (; str[i] && str[i] != '\n' && str[i] != '/'; i++)
		m = m * 10 + str[i] - '0';
	for (i++; str[i] && str[i] != '\n' && str[i] != '/'; i++)
		d = d * 10 + str[i] - '0';
	for (i++; str[i] && str[i] != '\n' && str[i] != '/'; i++)
		y = y * 10 + str[i] - '0';
	free(str);
	if (y <= 1900 || y > 2100 || m <= 0 || m > 12 || d <= 0 ||
		(d > months[m - 1] && !(m == 2 && d == 29 && is_a_leap_year(y))))
		return NULL;
	date* new_date = (date*)malloc(sizeof(date));
	new_date->day = d;
	new_date->month = m;
	new_date->year = y;
	return new_date;
}
song set_song()
{
	song new_one;
	printf("���������� �������� �����: ");
	new_one.name = get_string(stdin);
	if (length_of(new_one.name) == 2)
	{
		free(new_one.name);
		new_one.name = "��� ��������\n";
	}
	int lines_num;
	do
	{
		printf("\n������� ���������� ����� � ������ �����:");
		lines_num = get_int();
	} while (lines_num < 0 || lines_num > MAX_NUM_OF_LYR_LINES);
	if (lines_num == 0)
	{
		new_one.lyrics = (text*)malloc(sizeof(text));
		new_one.lyrics->num_of_lines = 0;
		new_one.lyrics->lines = NULL;
	}
	else
	{
		printf("\n������� �����:\n ");
		new_one.lyrics = get_text(stdin, yes_function, lines_num);
	}
	return new_one;
}
//------------------
void report_merror()
{
	printf("\n������ ������\n");
	exit(EXIT_FAILURE);
}
void report_uerror()
{
	printf("\n���-�� ����� �� ��� �� ����� ������ �����\n");
	exit(EXIT_FAILURE);
}
void report_werror()
{
	printf("\n���-�� ����� �� ��� �� ����� ������ �����\n");
	exit(EXIT_FAILURE);
}
int length_of(char* str)
{
	if (str == NULL) return 0;
	int i = 0;
	for (; str[i]; i++) {}
	return i + 1;
}
int substr(char* sub, char* str)
{
	if (str == NULL) return -1;
	if (sub == NULL || length_of(sub) == 1) return 0;
	int i = 0;
	for (; str[i]; i++)
	{
		int j = 0;
		for (; sub[j] && str[i + j] && sub[j] == str[i + j]; j++) {}
		if (sub[j] == '\0') return i + j;
	}
	return -1;
}
int are_strs_same(char* first, char* second)
{
	int i = 0;
	for (; first[i] && second[i]; i++)
	{
		if (first[i] != second[i]) return 0;
	}
	if (first[i] || second[i]) return 0;
	return 1;
}
int is_among(int* items, int num_of_items, int item)
{
	int i = 0;
	for (; i < num_of_items; i++)
		if (item == items[i]) return 1;
	return 0;
}
void out_str_wo_nl(char* str)
{
	if (str == NULL) return;
	int i = 0;
	for (; str[i] && str[i] != '\n'; i++)
		printf("%c", str[i]);
}
//--------------
int id_exists(id one)
{
	int i = 0;
	for (; i < num_of_users; i++)
		if (users[i]->user_id == one) return 1;
	for (; i < num_of_groups; i++)
		if (groups[i]->image->user_id == one) return 1;
	for (; i < num_of_releases; i++)
		if (releases[i]->release_id == one) return 1;
	return 0;
}
id get_new_id()
{
	id new_id;
	do {
		srand(time(NULL));
		new_id = rand();
	} while (id_exists(new_id));
	return new_id;
}
user* new_user(char* name)
{
	user* new_user = (user*)malloc(sizeof(user));
	new_user->user_id = get_new_id();
	new_user->name = name;
	new_user->releases = NULL;
	new_user->purchased_releases = NULL;
	new_user->full_account = new_user->num_of_purchased_releases = new_user->num_of_releases = new_user->revenue = 0;
	return new_user;
}
group* new_group(char* name, int num_of_members, id* members)
{
	group* new_group = (group*)malloc(sizeof(group));
	new_group->image = new_user(name);
	new_group->num_of_members = num_of_members;
	new_group->members = members;
	return new_group;
}
release* new_release()
{
	release* new_one = (release*)malloc(sizeof(release));
	new_one->release_id = get_new_id();
	return new_one;
}
box* new_box()
{
	box* new_one = (box*)malloc(sizeof(box));
	new_one->g = NULL;
	new_one->r = NULL;
	new_one->u = NULL;
	return new_one;
}
void delete_user(user* one)
{
	free(one->name);
	free(one->releases);
	free(one->purchased_releases);
	free(one);
}
void delete_group(group* one)
{
	delete_user(one->image);
	free(one->members);
	free(one);
}
void delete_text(text* one)
{
	int i = 0;
	for (; i < one->num_of_lines; i++)
		free(one->lines[i]);
	if (one->lines != NULL) free(one->lines);
	free(one);
}
void delete_song_info(song one)
{
	free(one.name);
	delete_text(one.lyrics);
}
void delete_release(release* one)
{
	free(one->name);
	free(one->authors);
	free(one->dropped);
	int i = 0;
	for (; i < one->num_of_songs; i++)
		delete_song_info(one->songs[i]);
	free(one->songs);
}
//----------
char* get_string(FILE* file_or_con)
{
	char* str;
	int chinput;
	size_t len = 0;
	size_t capacity = DEF_LINE_LEN;
	str = (char*)malloc(sizeof(char) * capacity);
	if (str == NULL) report_merror();
	while ((chinput = fgetc(file_or_con)) != EOF && chinput != '\n')
	{
		str[len++] = chinput;
		if (len == capacity)
		{
			str = (char*)realloc(str, sizeof(char) * (capacity *= 2));
			if (str == NULL) report_merror();
		}
	}
	str[len++] = '\n';
	if (len == capacity)
	{
		str = (char*)realloc(str, sizeof(char) * (capacity += 1));
		if (str == NULL) report_merror();
		str[len++] = '\0';
		return str;
	}
	str[len++] = '\0';
	return (char*)realloc(str, sizeof(char) * len);
}
void write_string_to_file(char* str, FILE* file)
{
	if (fputs(str, file) == EOF) report_werror();
}
text* get_text(FILE* file_or_con, int (*condition_for_reading)(), int max_line_num)
{
	char** lines;
	char* line;
	size_t line_num = 0;
	size_t capacity = DEF_LINE_NUM;
	lines = (char**)malloc(sizeof(char*) * capacity);
	if (lines == NULL)
	{
		printf("\n������ ������.\n");
		exit(EXIT_FAILURE);
	}
	while (condition_for_reading() && (max_line_num == 0 || line_num < max_line_num))
	{
		lines[line_num++] = get_string(file_or_con);
		if (line_num == capacity)
		{
			lines = (char**)realloc(lines, sizeof(char*) * (capacity *= 2));
			if (lines == NULL)
			{
				printf("\n������ ������.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	lines = (char**)realloc(lines, sizeof(char*) * line_num);
	text* output = (text*)malloc(sizeof(text));
	output->num_of_lines = line_num;
	output->lines = lines;
	return output;
}
void move_text_to_file(FILE* file, text* text)
{
	if (file == NULL || text == NULL) return;
	if (fprintf(file, "%d ", text->num_of_lines) == EOF) report_werror();
	int i = 0;
	for (; i < text->num_of_lines; i++)
		if (fputs(text->lines[i], file) == EOF) report_werror();
}
void get_data_from_file()
{
	FILE* db;
	errno_t err;
	err = fopen_s(&db, file_name, "rb");
	if (db == NULL)
	{
		num_of_groups = num_of_releases = num_of_users = 0;
		groups = NULL;
		releases = NULL;
		users = NULL;
		return;
	}
	if (fscanf_s(db, "%d %d %d ", &num_of_groups, &num_of_releases, &num_of_users) == EOF)
		report_uerror();
	int i, capacity;
	capacity = DEF_STRUCT_CAP;
	groups = (group**)malloc(sizeof(group*) * capacity);
	if (groups == NULL) report_merror();
	for (i = 0; i < num_of_groups;)
	{
		groups[i++] = get_group_from_file(db);
		if (i == capacity)
		{
			groups = (group**)realloc(groups, sizeof(group*) * (capacity *= 2));
			if (groups == NULL) report_merror();
		}
	}
	capacity = DEF_STRUCT_CAP;
	releases = (release**)malloc(sizeof(release*) * capacity);
	if (releases == NULL) report_merror();
	for (i = 0; i < num_of_releases;)
	{
		releases[i++] = get_release_from_file(db);
		if (i == capacity)
		{
			releases = (release**)realloc(releases, sizeof(release*) * (capacity *= 2));
			if (releases == NULL) report_merror();
		}
	}
	capacity = DEF_STRUCT_CAP;
	users = (user**)malloc(sizeof(user*) * capacity);
	if (users == NULL) report_merror();
	for (i = 0; i < num_of_users;)
	{
		users[i++] = get_user_from_file(db);
		if (i == capacity)
		{
			users = (user**)realloc(users, sizeof(user*) * (capacity *= 2));
			if (users == NULL) report_merror();
		}
	}
	fclose(db);
}
void write_data_to_file()
{
	FILE* db;
	errno_t err;
	err = fopen_s(&db, file_name, "wb");
	if (db == NULL)
	{
		report_werror();
	}
	if (fprintf(db, "%d %d %d ", num_of_groups, num_of_releases, num_of_users) == EOF) 	report_werror();
	int i;
	for (i = 0; i < num_of_groups; i++)
	{
		write_group_to_file(groups[i], db);
	}
	for (i = 0; i < num_of_releases; i++)
	{
		write_release_to_file(releases[i], db);
	}
	for (i = 0; i < num_of_users; i++)
	{
		write_user_to_file(users[i], db);
	}
	fclose(db);
}
user* get_user_from_file(FILE* file)
{
	user* new_one = (user*)malloc(sizeof(user));
	if (fscanf_s(file, "%d ", &new_one->user_id) == EOF) report_uerror();
	new_one->name = get_string(file);
	if (fscanf_s(file, "%d ", &new_one->num_of_releases) == EOF) report_uerror();
	new_one->releases = get_int_arr(file, new_one->num_of_releases);
	if (fscanf_s(file, "%d ", &new_one->num_of_purchased_releases) == EOF) report_uerror();
	new_one->purchased_releases = get_int_arr(file, new_one->num_of_purchased_releases);
	if (fscanf_s(file, "%d %d ", &new_one->revenue, &new_one->full_account) == EOF) report_uerror();
	return new_one;
}
void write_user_to_file(user* one, FILE* file)
{
	if (fprintf(file, "%d ", one->user_id) == EOF) report_werror();
	write_string_to_file(one->name, file);
	if (fprintf(file, "%d ", one->num_of_releases) == EOF) report_werror();
	write_int_arr_to_file(file, one->releases, one->num_of_releases);
	if (fprintf(file, "%d ", one->num_of_purchased_releases) == EOF) report_werror();
	write_int_arr_to_file(file, one->purchased_releases, one->num_of_purchased_releases);
	if (fprintf(file, "%d %d ", one->revenue, one->full_account) == EOF) report_werror();
}
group* get_group_from_file(FILE* file)
{
	group* new_one = (group*)malloc(sizeof(group));
	new_one->image = get_user_from_file(file);
	if (fscanf_s(file, "%d ", &new_one->num_of_members) == EOF) report_uerror();
	new_one->members = get_int_arr(file, new_one->num_of_members);
	return new_one;
}
void write_group_to_file(group* one, FILE* file)
{
	write_user_to_file(one->image, file);
	if (fprintf(file, "%d ", one->num_of_members) == EOF) report_werror();
	write_int_arr_to_file(file, one->members, one->num_of_members);
}
release* get_release_from_file(FILE* file)
{
	release* new_one = (release*)malloc(sizeof(release));
	if (fscanf_s(file, "%d %d ", &new_one->release_id, &new_one->type) == EOF) report_uerror();
	new_one->dropped = get_date(file);
	new_one->name = get_string(file);
	if (fscanf_s(file, "%d ", &new_one->num_of_authors) == EOF) report_uerror();
	new_one->authors = get_int_arr(file, new_one->num_of_authors);
	if (fscanf_s(file, "%d ", &new_one->num_of_songs) == EOF) report_uerror();
	new_one->songs = (song*)malloc(sizeof(song) * new_one->num_of_songs);
	int i;
	for (i = 0; i < new_one->num_of_songs; i++)
		new_one->songs[i] = get_song(file);
	if (fscanf_s(file, "%d ", &new_one->price) == EOF) report_uerror();
	return new_one;
}
void write_release_to_file(release* one, FILE* file)
{
	if (fprintf(file, "%d %d ", one->release_id, one->type) == EOF) report_werror();
	write_date_to_file(file, one->dropped);
	write_string_to_file(one->name, file);
	if (fprintf(file, "%d ", one->num_of_authors) == EOF) report_werror();
	write_int_arr_to_file(file, one->authors, one->num_of_authors);
	if (fprintf(file, "%d ", one->num_of_songs) == EOF) report_werror();
	int i;
	for (i = 0; i < one->num_of_songs; i++)
		write_song_to_file(file, one->songs[i]);
	if (fprintf(file, "%d ", one->price) == EOF) report_werror();
}
int* get_int_arr(FILE* file, int size)
{
	int* output = (int*)malloc(sizeof(int) * size);
	int i;
	for (i = 0; i < size; i++)
		if (fscanf_s(file, "%d ", &output[i]) == EOF) report_uerror();
	return output;
}
void write_int_arr_to_file(FILE* file, int* arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
		if (fprintf(file, "%d ", arr[i]) == EOF) report_werror();
}
void write_money_to_file(FILE* file, money one)
{
	if (fprintf(file, "%d ", one) == EOF) report_werror();
}
date* get_date(FILE* file)
{
	date* new_date = (date*)malloc(sizeof(date));
	if (fscanf_s(file, "%d %d %d ", &new_date->month, &new_date->day, &new_date->year) == EOF) report_uerror();
	return new_date;
}
void write_date_to_file(FILE* file, date* one)
{
	if (fprintf(file, "%d %d %d ", one->month, one->day, one->year) == EOF) report_werror();
}
song get_song(FILE* file)
{
	song new_one;
	new_one.name = get_string(file);
	int tmp;
	if (fscanf_s(file, "%d ", &tmp) == EOF) report_uerror();
	if (tmp != 0) new_one.lyrics = get_text(file, yes_function, tmp);
	else
	{
		new_one.lyrics = (text*)malloc(sizeof(text));
		new_one.lyrics->lines = NULL;
		new_one.lyrics->num_of_lines = 0;
	}
	return new_one;
}
void write_song_to_file(FILE* file, song one)
{
	write_string_to_file(one.name, file);
	move_text_to_file(file, one.lyrics);
}
int yes_function()
{
	return 1;
}
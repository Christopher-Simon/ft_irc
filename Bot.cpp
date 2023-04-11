#include "Bot.hpp"

//ordonne:
//0
//...
//5

Bot::Bot()
{
	id = ":BOT!BOT@BOT PRIVMSG ";
}

Bot::~Bot()
{
}

void Bot::send_welcome(Client &clt, Server &serv)
{
	std::string prev = id + clt.get_nick() + ":";
	serv.send_msg(prev + " ____  __ __  ____ _____ _____  ____  ____     __    ___       ___   __ __   ____  ______  ____     ___ ", clt.getfd());
	serv.send_msg(prev + "|    \\|  |  ||    / ___// ___/ /    ||    \\   /  ]  /  _]     /   \\ |  |  | /    ||      ||    \\   /  _]", clt.getfd());
	serv.send_msg(prev + "|  o  )  |  | |  (   \\_(   \\_ |  o  ||  _  | /  /  /  [_     |     ||  |  ||  o  ||      ||  D  ) /  [_ ", clt.getfd());
	serv.send_msg(prev + "|   _/|  |  | |  |\\__  |\\__  ||     ||  |  |/  /  |    _]    |  Q  ||  |  ||     ||_|  |_||    / |    _]", clt.getfd());
	serv.send_msg(prev + "|  |  |  :  | |  |/  \\ |/  \\ ||  _  ||  |  /   \\_ |   [_     |     ||  :  ||  _  |  |  |  |    \\ |   [_ ", clt.getfd());
	serv.send_msg(prev + "|  |  |     | |  |\\    |\\    ||  |  ||  |  \\     ||     |    |     ||     ||  |  |  |  |  |  .  \\|     |", clt.getfd());
	serv.send_msg(prev + "|__|   \\__,_||____|\\___| \\___||__|__||__|__|\\____||_____|     \\__,_| \\__,_||__|__|  |__|  |__|\\_||_____|", clt.getfd());
	serv.send_msg(prev + "", clt.getfd());
}

std::vector<std::string> Bot::new_board()
{
	std::vector<std::string> board;
	for (int i = 0; i < 6; i++)
		board.push_back("       ");
	return board;
}

void Bot::print_msg(Client &clt, Server &serv, std::string msg)
{
	std::string prev = id + clt.get_nick() + " :";
	serv.send_msg(prev + msg, clt.getfd());
}

void Bot::print_board(Client &clt, Server &serv)
{
	print_msg(clt, serv, "  1   2   3   4   5   6   7  ");
	print_msg(clt, serv, "+---+---+---+---+---+---+---+");
	for (int i = 0; i <= 5; i++)
	{
		std::string line = "|";
		for (int j = 0; j < 7; j++)
		{
			line = line + " " + memory[clt.getfd()][i][j] + " |";
		}
		print_msg(clt, serv, line);
		print_msg(clt, serv, "+---+---+---+---+---+---+---+");
	}
	print_msg(clt, serv, "  1   2   3   4   5   6   7  ");
}

int Bot::add_piece(Client &clt, int pos, char car, Server &serv)
{
	int h = 5;
	while(h >= 0 && memory[clt.getfd()][h][pos] != ' ')
		h--;
	if (h < 0)
	{
		print_msg(clt, serv, "Colomn already full");
		return -1;
	}
	else
		memory[clt.getfd()][h][pos] = car;
	return 0;
}

int Bot::check_victory(char car, Client &clt)
{
    //Colonnes;
    for (int j=0; j<7; j++)
    {
        int i=5;
        while (i>2)
        {
            if (memory[clt.getfd()][i][j]==car && memory[clt.getfd()][i-1][j]==car && memory[clt.getfd()][i-2][j]==car && memory[clt.getfd()][i-3][j]==car)
                return 1;
            i--;
        }
    }
    //Lignes;
    for (int i=5; i>=0; i--)
    {
        int j=0;
        while (j<5)
        {
            if (memory[clt.getfd()][i][j]==car && memory[clt.getfd()][i][j+1]==car && memory[clt.getfd()][i][j+2]==car && memory[clt.getfd()][i][j+3]==car)
                return 1;
            j++;
        }
    }
    // //Diagonales
    char z= car;
    //Diagonales de 4
    int diag_4[4][4][2]={{{2,0},{3,1},{4,2},{5,3}},{{0,3},{1,4},{2,5},{3,6}},{{0,3},{1,2},{2,1},{3,0}},{{2,6},{3,5},{4,4},{5,3}}};
    for(int s=0; s<4; s++)
    {
        if (memory[clt.getfd()][diag_4[s][0][0]][diag_4[s][0][1]]==z && memory[clt.getfd()][diag_4[s][1][0]][diag_4[s][1][1]]==z && memory[clt.getfd()][diag_4[s][2][0]][diag_4[s][2][1]]==z && memory[clt.getfd()][diag_4[s][3][0]][diag_4[s][3][1]]==z)
            return 1;
    }
    //Diagonales de 5
    int diag_5[4][5][2]={{{1,0},{2,1},{3,2},{4,3},{5,4}},{{0,2},{1,3},{2,4},{3,5},{4,6}},{{0,4},{1,3},{2,2},{3,1},{4,0}},{{1,6},{2,5},{3,4},{4,3},{5,2}}};
    int a=0;
    while (a<2)
    {
        for(int s=0; s<4; s++)
        {
            if (memory[clt.getfd()][diag_5[s][a][0]][diag_5[s][a][1]]==z && memory[clt.getfd()][diag_5[s][a+1][0]][diag_5[s][a+1][1]]==z && memory[clt.getfd()][diag_5[s][a+2][0]][diag_5[s][a+2][1]]==z && memory[clt.getfd()][diag_5[s][a+3][0]][diag_5[s][a+3][1]]==z)
                return 1;
        }
        a++;
    }
    //Diagonales de 6
    int diag_6[4][6][2]={{{0,0},{1,1},{2,2},{3,3},{4,4},{5,5}},{{0,1},{1,2},{2,3},{3,4},{4,5},{5,6}},{{0,5},{1,4},{2,3},{3,2},{4,1},{5,0}},{{0,6},{1,5},{2,4},{3,3},{4,2},{5,1}}};
    a=0;
    while (a<3)
    {
        for(int s=0; s<4; s++)
        {
            if (memory[clt.getfd()][diag_6[s][a][0]][diag_6[s][a][1]]==z && memory[clt.getfd()][diag_6[s][a+1][0]][diag_6[s][a+1][1]]==z && memory[clt.getfd()][diag_6[s][a+2][0]][diag_6[s][a+2][1]]==z && memory[clt.getfd()][diag_6[s][a+3][0]][diag_6[s][a+3][1]]==z)
                return 1;
        }
        a++;
    }
    return 0;
}

int Bot::possibilites(int p, Client &clt)
{
    int i=5;
    p-=1;
    while (i >= 0 && (memory[clt.getfd()][i][p]=='X' || memory[clt.getfd()][i][p]=='O'))
        i--;
    if (i<0)
        return 0;
    //Colonnes
    int compteur_colonne = 1;
    int intermed=i+1;
    char controller=' ';
    if (intermed<6 && (memory[clt.getfd()][intermed][p]=='X' || memory[clt.getfd()][intermed][p]=='O'))
    {
        compteur_colonne++;
        controller=memory[clt.getfd()][intermed][p];
        intermed++;
    }
    while(intermed<6 && memory[clt.getfd()][intermed][p]==controller)
    {
        compteur_colonne++;
        intermed++;
    }
    //Lignes
    int compteur_ligne=1;
    int compteur_ligne_left = 1;
    intermed=p-1;
    char controller_left='d';
    if (intermed>=0 && (memory[clt.getfd()][i][intermed]=='X' || memory[clt.getfd()][i][intermed]=='O'))
    {
        compteur_ligne_left++;
        controller_left=memory[clt.getfd()][i][intermed];
        intermed--;
    }
    while(intermed>=0 && memory[clt.getfd()][i][intermed]==controller_left)
    {
        compteur_ligne_left++;
        intermed--;
    }
    int compteur_ligne_right = 1;
    intermed=p+1;
    char controller_right='d';
    if (intermed<7 && (memory[clt.getfd()][i][intermed]=='X' || memory[clt.getfd()][i][intermed]=='O'))
    {
        compteur_ligne_right++;
        controller_right=memory[clt.getfd()][i][intermed];
        intermed++;
    }
    while(intermed<7 && memory[clt.getfd()][i][intermed]==controller_right)
    {
        compteur_ligne_right++;
        intermed++;
    }
    if (controller_right==controller_left)
        compteur_ligne=compteur_ligne_left+compteur_ligne_right-1;
    else
        (compteur_ligne_left>compteur_ligne_right)?(compteur_ligne=compteur_ligne_left):(compteur_ligne=compteur_ligne_right);
    int compteur_col_ligne=(compteur_ligne>compteur_colonne)?compteur_ligne:compteur_colonne;
    //Diagonales
    int compteur_diag=1;
    int compteur_diag_droit=1;
    int compteur_diag_gauche=1;
    //Diagonale gauche haute
    int compteur_diag_left_haut=1;
    int intermed_col=p-1;
    int intermed_ligne=i-1;
    char controller_left_haut='d';
    if (intermed_col>=0 && intermed_ligne>=0 && (memory[clt.getfd()][intermed_ligne][intermed_col]=='X' || memory[clt.getfd()][intermed_ligne][intermed_col]=='O'))
    {
        compteur_diag_left_haut++;
        controller_left_haut=memory[clt.getfd()][intermed_ligne][intermed_col];
        intermed_col--;
        intermed_ligne--;
    }
    while(intermed>=0 && intermed_ligne>=0 && memory[clt.getfd()][intermed_ligne][intermed_col]==controller_left_haut)
    {
        compteur_diag_left_haut++;
        intermed_col--;
        intermed_ligne--;
    }
    //Diagonale gauche basse
    int compteur_diag_left_bas=1;
    intermed_col=p-1;
    intermed_ligne=i+1;
    char controller_left_bas='d';
    if (intermed_col>=0 && intermed_ligne<6 && (memory[clt.getfd()][intermed_ligne][intermed_col]=='X' || memory[clt.getfd()][intermed_ligne][intermed_col]=='O'))
    {
        compteur_diag_left_bas++;
        controller_left_bas=memory[clt.getfd()][intermed_ligne][intermed_col];
        intermed_col--;
        intermed_ligne++;
    }
    while(intermed>=0 && intermed_ligne<6 && memory[clt.getfd()][intermed_ligne][intermed_col]==controller_left_bas)
    {
        compteur_diag_left_bas++;
        intermed_col--;
        intermed_ligne++;
    }
    //Diagonale droite haute
    int compteur_diag_right_haut=1;
    intermed_col=p+1;
    intermed_ligne=i-1;
    char controller_right_haut='d';
    if (intermed_col<7 && intermed_ligne>=0 && (memory[clt.getfd()][intermed_ligne][intermed_col]=='X' || memory[clt.getfd()][intermed_ligne][intermed_col]=='O'))
    {
        compteur_diag_right_haut++;
        controller_right_haut=memory[clt.getfd()][intermed_ligne][intermed_col];
        intermed_col++;
        intermed_ligne--;
    }
    while(intermed<7 && intermed_ligne>=0 && memory[clt.getfd()][intermed_ligne][intermed_col]==controller_right_haut)
    {
        compteur_diag_right_haut++;
        intermed_col++;
        intermed_ligne--;
    }
    //Diagonale droite basse
    int compteur_diag_right_bas=1;
    intermed_col=p+1;
    intermed_ligne=i+1;
    char controller_right_bas='d';
    if (intermed_col<7 && intermed_ligne<6 && (memory[clt.getfd()][intermed_ligne][intermed_col]=='X' || memory[clt.getfd()][intermed_ligne][intermed_col]=='O'))
    {
        compteur_diag_right_bas++;
        controller_right_bas=memory[clt.getfd()][intermed_ligne][intermed_col];
        intermed_col++;
        intermed_ligne++;
    }
    while(intermed<7 && intermed_ligne<6 && memory[clt.getfd()][intermed_ligne][intermed_col]==controller_right_bas)
    {
        compteur_diag_right_bas++;
        intermed_col++;
        intermed_ligne++;
    }
    if (controller_right_bas==controller_left_haut)
        compteur_diag_gauche=compteur_diag_left_haut+compteur_diag_right_bas-1;
    else
        (compteur_diag_left_haut>compteur_diag_right_bas)?(compteur_diag_gauche=compteur_diag_left_haut):(compteur_diag_gauche=compteur_diag_right_bas);
    if (controller_right_haut==controller_left_bas)
        compteur_diag_droit=compteur_diag_left_bas+compteur_diag_right_haut-1;
    else
        (compteur_diag_left_bas>compteur_diag_right_haut)?(compteur_diag_droit=compteur_diag_left_bas):(compteur_diag_droit=compteur_diag_right_haut);
    (compteur_diag_droit>compteur_diag_gauche)?(compteur_diag=compteur_diag_droit):(compteur_diag=compteur_diag_gauche);
    return (compteur_col_ligne>compteur_diag)?(compteur_col_ligne):(compteur_diag);
}

static double nb_aleatoire(void)
{
	return rand() / (RAND_MAX + 1.);
}


static int nb_aleatoire_entre(int min, int max)
{
	return nb_aleatoire() * (max - min + 1) + min;
}

int Bot::meilleur_coup(Client &clt)
{
    int score[7]={0,0,0,0,0,0,0};
    int max=0;
    for (int i=0;i<7;i++)
    {
		score[i]=possibilites(i+1, clt);
		if (score[i] > max)
			max = score[i];
    }
    int n=0;
    for (int j=0;j<7;j++)
        {
            if (score[j]==max)
                n++;
        }
    int possib[n];
    int indice=0;
    for (int j=0;j<7;j++)
        {
            if (score[j]==max)
            {
                possib[indice]=j+1;
                indice++;
            }
        }
    if (n==1)
        return possib[0];
    return possib[nb_aleatoire_entre(0,n-1)];
}

int Bot::check_full(Client &clt)
{
	for (int i = 0; i <= 5; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (memory[clt.getfd()][i][j] == ' ')
				return 0;
		}
	}
	return 1;
}

void Bot::handler(Client &clt, Server &serv, std::string msg)
{
	if ((msg == ":play" || msg == ":new game") && memory.find(clt.getfd()) == memory.end())
	{
		memory[clt.getfd()] = new_board();
		send_welcome(clt, serv);
		print_board(clt, serv);
	}
	else if (memory.find(clt.getfd()) != memory.end() && msg.size() == 2 && msg[1] >= '1' && msg[1] <= '7')
	{
		if (add_piece(clt, static_cast<int>(msg[1] - '0') - 1, 'X', serv) == -1)
			return;
		print_board(clt, serv);
		if (check_victory('X', clt)== 1)
		{
			print_msg(clt, serv, "Victoire du joueur");
			memory.erase(clt.getfd());
			return;
		}
		if (check_full(clt) == 1)
		{
			print_msg(clt, serv, "Match nul");
			memory.erase(clt.getfd());
			return;
		}
		if (add_piece(clt, meilleur_coup(clt) - 1, 'O', serv) == -1)
			return;
		print_board(clt, serv);
		if (check_victory('O', clt)== 1)
		{
			print_msg(clt, serv, "Victoire de l'ordi");
			memory.erase(clt.getfd());
			return;
		}
		if (check_full(clt) == 1)
		{
			print_msg(clt, serv, "Match nul");
			memory.erase(clt.getfd());
			return;
		}
	}
	else if (memory.find(clt.getfd()) != memory.end() && msg == "abandon")
	{
		print_msg(clt, serv, "You abandoned this game");
		memory.erase(clt.getfd());
	}
}
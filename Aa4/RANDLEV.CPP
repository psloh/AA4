#include <fstream.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



inline int max(int a, int b)
{
	return a>b?a:b;
}

int main(void)
{
	char filename[301];
	int num;
	int max_type;
	int rows;
	int start;
	int max_path;
	int max_carries;
	int max_cluster_size;
	int i, j;
	ofstream outf;
	int row;
	int e_type, e_pos_x, e_pos_y, e_path, e_carries;
	int cluster_size;
	int cluster_center;
	int enemy_num;
	ofstream LOGfile;
	int background;
	char modelname[80];
	long temp, temp2;
	char tempstring[80];

	cout << "Number of levels to make: ";
	cin >> num;
	cout << "Maximum enemy type: ";
	cin >> max_type;
	cout << "Maximum path number: ";
	cin >> max_path;
	cout << "Maximum carries number: ";
	cin >> max_carries;
	cout << "Number of rows in level (number of rows 16 pix long): ";
	cin >> rows;
	cout << "Starting file number: ";
	cin >> start;
	cout << "Maximum cluster size: ";
	cin >> max_cluster_size;
	cout << "Model file name: ";
	cin.getline(model);
	ifstream LevFile(model);

// new


	ifstream LevFile(filename);          // Level file.

//    ;music description block
	while (LevFile.peek() != '=')  LevFile.ignore();    // TRACK=
	LevFile.ignore();
	LevFile >> temp;   // get the number of different scenery tiles
	outf << "TRACK=" << temp << endl << endl;

//    ;level description block
	while (LevFile.peek() != '=')  LevFile.ignore();   // LINES=
	LevFile.ignore();

	LevFile >> temp;    // get the number of lines in the level description.

	outf << "LINES=" << temp << endl;

	LevFile.ignore();
	for (i = 0; i < L.ld_lines; i++)               // get the level description
	{
		LevFile.getline(tempstring, 79);
		outf << tempstring << endl;
	}

	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF SCENERY TILES=
	LevFile.ignore();

	LevFile >> temp;   // get the number of different scenery tiles
	outf << endl << "NUMBER OF SCENERY TILES= " << temp << endl;

	while (LevFile.peek() != '=')  LevFile.ignore();    // LEVEL GRAPHICS FILE=
	LevFile.ignore();
	LevFile.getline(tempstring, 299);   // get level graphics filename
	outf << "LEVEL GRAPHICS FILE=" << tempstring << endl;

	while (LevFile.peek() != '=')  LevFile.ignore();    // LEVEL MODULE FILE=
	LevFile.ignore();
	LevFile.getline(tempstring, 299);   // get level module filename
	outf << "LEVEL MODULE FILE=" << tempstring << endl;



//----------------------------------------------------
//              input level strips

	while (LevFile.peek() != '=')  LevFile.ignore();    // NUMBER OF SCENERY STRIPS=
	LevFile.ignore();

	LevFile >> temp;   // get the number of different level strips
	outf << "NUMBER OF SCENERY STRIPS=" << temp;

	for (i = 0; i < temp; i++)      // insert all level strips into linked list
	{
		for (j = 0; j < 20; j++)          // put the data in each node
		{
			LevFile >> temp2;
			outf << temp2;
		}
		outf << endl;
	}

//--------------------------------------------------
//                end level strip input

//--------------------------------------------------
//                load scenery list block

	while (LevFile.peek() != '=')  LevFile.ignore();   // NUMBER OF ROWS=
	LevFile.ignore();

	LevFile >> num_rows;
	L.lev_rows = num_rows*16;
	outf << "NUMBER OF ROWS=" << num_rows << endl;

	for (i = 0; i < num_rows; i++)
		LevFile >> L.lev_strips[i];

//--------------------------------------------------
//                end scenery list block


// new

	for (i = 0; i < num; i++)
	{
		sprintf(filename, "lev%d.dat", start+i);
		outf.open(filename, ios::out);
		sprintf(filename, "lev%d.log", start+i);
		LOGfile.open(filename, ios::out);

		enemy_num = 1;
		row = 400;
		while (row < rows-30)
		{
			e_type = random(max_type+1);
			if (e_type == 3)
				cluster_size = 1;
			else
				cluster_size = random(max_cluster_size);
			cluster_center = cluster_size*16 + random(320-cluster_size*32);
			for (j = -cluster_size/2; j <= cluster_size/2; j++)
			{
				e_pos_x = cluster_center + j*32;
				e_pos_y = row;
				if (e_type == 3)
					e_path = 2;
				else
					e_path = random(max_path+1);
            if (!random(100))
					e_carries = 1;
            else if (!random(20))
            	e_carries = 0;
            else
            	e_carries = -1;
				outf << " ;ENEMY <" << enemy_num << ">" << endl;
				outf << " TYPE=" << e_type << endl;
				outf << " POSITION=" << e_pos_x << " " << e_pos_y << endl;
				outf << " PATH=" << e_path << endl;
				outf << " CARRIES=" << e_carries << endl;

				enemy_num++;
			}

         if (e_type == 12)
         	LOGfile << row << endl;

			if (e_type == 3)
				row += 200;
			else
				row += random(50) + cluster_size*25;
		}
      outf.close();
      LOGfile.close();
	}

	return 0;
}

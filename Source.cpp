#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void shape_Detection_and_Recognition()
{
	
	VideoCapture cap("video.MOV");
	Mat frame, frame_grey, frame_canny, frame_contour;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	vector<vector<Point> > contours;
	vector<vector<Point> > contours2;
	vector<Vec4i> hierarchy;
	

	while (true)
	{

		cap >> frame;
		if (frame.empty())
			cout << "Cette image est vide !" << endl;

		resize(frame, frame, Size(700, 700), 0, 0, INTER_LINEAR); // Je resize la taille des fenetres à ma convenance

		// Partie 1 :

		// Question 1 : Convertir l'image en niveau de gris
		cvtColor(frame, frame_grey, COLOR_BGR2GRAY);


		// Question 2 : Filtre de Cannny
		Canny(frame_grey, frame_canny, 80, 160, 3);


		// Question 3 : Dilatation
		dilate(frame_canny, frame_contour, kernel, Point(3, 3));


		// Question 4 : Détecter les contours
		findContours(frame_contour, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


		// Question 5 : Afficher les contours détectés 
		
		Mat drawing = frame;
		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(255, 0, 255);
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
		}
		
		// Partie 2 :
		// Question 1 : Douglas-Peucker

		contours2.resize(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			double epsilon = 0.02* arcLength(contours[i], true);
			approxPolyDP(contours[i], contours2[i], epsilon, true);
		}

		// Question 2 : Exclusion

		for (int i = 0; i < contours2.size(); i++)
		{
			if (contourArea(contours2[i]) > 5000 && isContourConvex(contours2[i]))
			{
				int x = int((contours2[i][0].x + contours2[i][1].x + contours2[i][2].x) / 4);
				int y = int((contours2[i][0].y + contours2[i][1].y + contours2[i][2].y) / 4);
				Scalar color2;
				string shape;

				// Question 3 : triangle
				if (contours2[i].size() == 3)
				{
					shape = "triangle";
					color2 = Scalar(0, 0, 255);
				}

				// Question 4 : Carré et Rectangle
				else if (contours2[i].size() == 4)
				{
					// Si la largeur du quadrilatère est = à sa longueur, alors c'est un carré
					if (boundingRect(contours2[i]).width == boundingRect(contours2[i]).height)
					{
						shape = "carre";
						color2 = Scalar(255, 255, 0);
					}
					// Sinon c'est un rectangle
					else
					{
						shape = "rectangle";
						color2 = Scalar(0, 255, 0);
					}
						
				}

				// Question 5 : Cercle
				else
				{
					shape = "cercle";
					color2 = Scalar(255, 0, 0);
				}

				putText(frame, shape, Point(x, y), 4, 2, (255, 255, 255));
				drawContours(drawing, contours2, i, color2, 2, 8, hierarchy, 0, Point());
			}
		}


		imshow("video_source", frame);



		if (waitKey(40) == 27) // C'est le code ASCII de la touche "Echap" du clavier
			break;
	}
}



int main()
{
	shape_Detection_and_Recognition(); // Parties 1 et 2
}

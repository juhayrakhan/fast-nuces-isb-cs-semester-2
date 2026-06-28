#include "Q3_Submission.h"
using namespace std;

int main() {
	const float anchor_positions[][3] = {
	{-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
	{-1,-1,1}, {1,-1,1}, {1,1,1}, {-1,1,1}
	};

	const int link_pairs[][2] = {
	{0,1},{1,2},{2,3},{3,0},
	{4,5},{5,6},{6,7},{7,4},
	{0,4},{1,5},{2,6},{3,7}
	};

	int anchor_count = 8;
	int link_count = 12;

	bool xyz[3] = { true,true,true }; // toggle for axis rottaion

	Model model(anchor_count, link_count);

	for (int i = 0; i < anchor_count; i++)
		model.anchors[i] = new Point3(anchor_positions[i][0], anchor_positions[i][1], anchor_positions[i][2]);

	for (int i = 0; i < link_count; i++) {
		int l1 = link_pairs[i][0], l2 = link_pairs[i][1];
		model.links[i] = new Line(model.anchors[l1], model.anchors[l2]);
	}

	for (int n = 0; n < loops; n++) {
		clear();
		
		for (int i = 0; i < model.anchorCount; i++)	rotate(model.anchors[i], speed);
		for (int i = 0; i < model.linkCount; i++) {
			double depth = (model.links[i]->p1->z + model.links[i]->p2->z) / 2;
			Point2 p1 = project(model.links[i]->p1);
			Point2 p2 = project(model.links[i]->p2);
			drawLine(&p1, &p2, depth);
		}
		border();
		renderScreen();
		
		wait(speed);
	}

	return 0;
}
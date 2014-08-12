#ifndef STATISTICS_H_DEFINED
#define STATISTICS_H_DEFINED

///
struct Statistics
{
	unsigned int lines;
	unsigned int pieces;

	unsigned int I;
	unsigned int T;
	unsigned int L;
	unsigned int J;
	unsigned int S;
	unsigned int Z;
	unsigned int O;

	unsigned int singles;
	unsigned int doubles;
	unsigned int triples;
	unsigned int tetris;

	Statistics():
		lines(0),
		pieces(0),
		I(0),
		T(0),
		L(0),
		J(0),
		S(0),
		Z(0),
		O(0),
		singles(0),
		doubles(0),
		triples(0),
		tetris(0)
	{ }
};

#endif //STATISTICS_H_DEFINED


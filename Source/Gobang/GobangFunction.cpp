#include "GobangFunction.h"

AGobangFunction::AGobangFunction() {
	PrimaryActorTick.bCanEverTick = true;

	int32 Cnt = 0;
	for (int32 i = 1; i <= 15; ++i) {
		for (int32 j = 1; j <= 11; ++j) {
			for (int32 k = 0; k < 5; ++k) {
				WinningArray[Cnt][k] = &Board[i][j + k];
				WinningArray[Cnt + 1][k] = &Board[j + k][i];
			}
			WinningArrayDic[Cnt] = {
				{i, j},
				{i, j + 4}
			};
			WinningArrayDic[Cnt + 1] = {
				{j, i},
				{j + 4, i}
			};
			Cnt += 2;
		}
	}
	for (int32 j = 1; j <= 11; ++j) {
		for (int32 k = 0; k < 5; ++k) {
			WinningArray[Cnt][k] = &Board[j + k][j + k];
			WinningArray[Cnt + 1][k] = &Board[j + k][16 - j - k];
		}
		WinningArrayDic[Cnt] = {
			{j, j},
			{j + 4, j + 4}
		};
		WinningArrayDic[Cnt + 1] = {
			{j, 16 - j},
			{j + 4, 12 - j}
		};
		Cnt += 2;
	}
	for (int32 i = 2; i <= 11; ++i) {
		for (int32 j = 1; j <= 12 - i; ++j) {
			for (int32 k = 0; k < 5; ++k) {
				WinningArray[Cnt][k] = &Board[i + j + k - 1][j + k];
				WinningArray[Cnt + 1][k] = &Board[j + k][i + j + k - 1];
				WinningArray[Cnt + 2][k] = &Board[17 - i - j - k][j + k];
				WinningArray[Cnt + 3][k] = &Board[16 - j - k][i + j + k - 1];
			}
			WinningArrayDic[Cnt] = {
				{i + j - 1, j},
				{i + j + 3, j + 4}
			};
			WinningArrayDic[Cnt + 1] = {
				{j, i + j - 1},
				{j + 4, i + j + 3}
			};
			WinningArrayDic[Cnt + 2] = {
				{17 - i - j, j},
				{13 - i - j, j + 4}
			};
			WinningArrayDic[Cnt + 3] = {
				{16 - j, i + j - 1},
				{12 - j, i + j + 3}
			};
			Cnt += 4;
		}
	}
	IsValid[8][8] = 1;
	Key = 13107947371880225101;
}

void AGobangFunction::Initialize() {
	Turn = 1;
	Board.assign(17, vector<int32>(17, 0));
	IsValid.assign(17, vector<int32>(17, 0));
	IsValid[8][8] = 1;
	while (!Record.empty())
		Record.pop();
}

int32 AGobangFunction::GetTurn() {
	if (Record.size() == 255)
		return 0;
	else
		return Turn;
}

TArray<int32> AGobangFunction::GetBoard() {
	TArray<int32> Res;
	for (int32 i = 1; i <= 15; ++i)
		for (int32 j = 1; j <= 15; ++j)
			Res.Add(Board[i][j]);
	return Res;
}

stack<pair<int32, int32>> AGobangFunction::GetRecord() {
	return Record;
}

void AGobangFunction::GetPrevPiece(int32& X, int32& Y) {
	if (Record.size() == 0) {
		X = -1;
		Y = -1;
	} else {
		X = Record.top().first;
		Y = Record.top().second;
	}
}

int32 AGobangFunction::Check(int32 X, int32 Y) {
	const pair<int32, int32> p{X, Y};
	if (p.first < 1 || p.first > 15 || p.second < 1 || p.second > 15)
		return -1;
	return Board[p.first][p.second];
}

int32 AGobangFunction::Drop(int32 X, int32 Y) {
	const pair<int32, int32> p{X, Y};
	if (p.first < 1 || p.first > 15 || p.second < 1 || p.second > 15 || Board[p.first][p.second])
		return 1;
	Board[p.first][p.second] = Turn;
	if (Turn == 1)
		Key ^= Board1[p.first][p.second];
	else
		Key ^= Board2[p.first][p.second];
	for (int32 i = max(1, p.first - 2); i <= min(15, p.first + 2); ++i)
		for (int32 j = max(1, p.second - 2); j <= min(15, p.second + 2); ++j)
			++IsValid[i][j];
	Record.push(p);
	Turn = 3 - Turn;
	return 0;
}

void AGobangFunction::Undo() {
	if (Record.empty())
		return;
	pair<int32, int32> p = Record.top();
	Board[p.first][p.second] = 0;
	if (Turn == 2)
		Key ^= Board1[p.first][p.second];
	else
		Key ^= Board2[p.first][p.second];
	for (int32 i = max(1, p.first - 2); i <= min(15, p.first + 2); ++i)
		for (int32 j = max(1, p.second - 2); j <= min(15, p.second + 2); ++j)
			--IsValid[i][j];
	Record.pop();
	Turn = 3 - Turn;
}

int32 AGobangFunction::Judge() {
	for (int32 i = 0; i < 572; ++i) {
		int32 Temp = 1;
		for (int32 j = 0; j < 5; ++j)
			Temp *= *WinningArray[i][j];
		if (Temp == 1) {
			WinningPosition = i;
			return 1;
		} else if (Temp == 32) {
			WinningPosition = i;
			return 2;
		}
	}
	return 0;
}

void AGobangFunction::GetWinningPos(int32& X1, int32& Y1, int32& X2, int32& Y2) {
	const pair<pair<int32, int32>, pair<int32, int32>> Temp = WinningArrayDic[WinningPosition];
	X1 = Temp.first.first;
	Y1 = Temp.first.second;
	X2 = Temp.second.first;
	Y2 = Temp.second.second;
}

void AGobangFunction::Next(int32& X, int32& Y) {
	pair<int32, int32> Pos;
	if (Record.size() < 3 || Record.size() > 150)
		AlphaBeta(8, INT_MIN, INT_MAX, &Pos);
	else if (Record.size() > 170)
		AlphaBeta(8, INT_MIN, INT_MAX, &Pos);
	else
		AlphaBeta(10, INT_MIN, INT_MAX, &Pos);
	X = Pos.first;
	Y = Pos.second;
}

int32 AGobangFunction::GetNumOfPieces() {
	return static_cast<int32>(Record.size());
}

int32 AGobangFunction::AlphaBeta(int32 depth, int32 alpha, int32 beta, pair<int32, int32>* pos) {
	if (Judge() || !depth || !GetTurn())
		return EvaluateBoard();
	int32 score;
	vector<pair<int32, pair<int32, int32>>> candidates;
	for (int32 i = 1; i <= 15; ++i) {
		for (int32 j = 1; j <= 15; ++j) {
			if (!Board[i][j] && IsValid[i][j]) {
				Drop(i, j);
				candidates.push_back({EvaluatePoint({i, j}) - static_cast<int32>(sqrt((i - 8) * (i - 8) + (j - 8) * (j - 8))), {i, j}});
				Undo();
			}
		}
	}
	sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
		return a.first > b.first;
	});
	if (Turn == 1) {
		score = INT_MIN;
		for (int32 i = 0; i < candidates.size() && i < 7; ++i) {
			auto x = candidates[i].second;
			Drop(x.first, x.second);
			int32 res;
			if (Zobrist.count(Key) && Zobrist[Key].first >= depth)
				res = Zobrist[Key].second;
			else {
				res = AlphaBeta(depth - 1, alpha, beta) - static_cast<int32>(sqrt((x.first - 8) * (x.first - 8) + (x.second - 8) * (x.second - 8)));
				Zobrist[Key] = {depth, res};
			}
			if (res > score) {
				score = res;
				if (pos)
					*pos = x;
			}
			Undo();
			alpha = max(alpha, score);
			if (alpha >= beta)
				break;
		}
	} else {
		score = INT_MAX;
		for (int32 i = 0; i < candidates.size() && i < 7; ++i) {
			auto x = candidates[i].second;
			Drop(x.first, x.second);
			int32 res;
			if (Zobrist.count(Key) && Zobrist[Key].first >= depth)
				res = Zobrist[Key].second;
			else {
				res = AlphaBeta(depth - 1, alpha, beta) + static_cast<int32>(sqrt((x.first - 8) * (x.first - 8) + (x.second - 8) * (x.second - 8)));
				Zobrist[Key] = {depth, res};
			}
			if (res < score) {
				score = res;
				if (pos)
					*pos = x;
			}
			Undo();
			beta = min(beta, score);
			if (alpha >= beta)
				break;
		}
	}
	return score;
}

int32 AGobangFunction::EvaluateBoard() {
	int32 score = 0;
	for (int32 i = 1; i <= 15; ++i) {
		for (int32 j = 1; j <= 15; ++j) {
			if (Board[i][j]) {
				for (int32 direction = 1; direction <= 4; ++direction) {
					// 1:- 2:| 3:\ 4:/
					score += AnalysisLine(GetLine(direction, {i, j}), Board[i][j]) * (Board[i][j] == 1 ? 1 : -1);
				}
			}
		}
	}
	return score;
}

int32 AGobangFunction::EvaluatePoint(pair<int32, int32> pos) {
	int32 myScore = 0, opponentScore = 0;
	for (int32 direction = 1; direction <= 4; direction++) {
		// 1:- 2:| 3:\ 4:/
		const auto& line = GetLine(direction, pos);
		myScore += AnalysisPoint_My(line, Turn);
		opponentScore += AnalysisPoint_Adversary(line, Turn);
	}
	return myScore + opponentScore;
}

vector<int32> AGobangFunction::GetLine(int32 direction, pair<int32, int32> pos) {
	vector<int32> line(12, -1);
	switch (direction) {
	case 1:
		for (int32 i = -5; i <= 5; ++i)
			if (pos.second + i >= 1 && pos.second + i <= 15)
				line[i + 5] = Board[pos.first][pos.second + i];
			else
				line[i + 5] = -1;
		break;
	case 2:
		for (int32 i = -5; i <= 5; ++i)
			if (pos.first + i >= 1 && pos.first + i <= 15)
				line[i + 5] = Board[pos.first + i][pos.second];
			else
				line[i + 5] = -1;
		break;
	case 3:
		for (int32 i = -5; i <= 5; ++i)
			if (pos.first + i >= 1 && pos.second + i >= 1 && pos.first + i <= 15 && pos.second + i <= 15)
				line[i + 5] = Board[pos.first + i][pos.second + i];
			else
				line[i + 5] = -1;
		break;
	case 4:
		for (int32 i = -5; i <= 5; ++i)
			if (pos.first + i >= 1 && pos.second - i >= 1 && pos.first + i <= 15 && pos.second - i <= 15)
				line[i + 5] = Board[pos.first + i][pos.second - i];
			else
				line[i + 5] = -1;
		break;
	default:
		break;
	}
	return line;
}

int32 AGobangFunction::AnalysisLine(const vector<int32>& line, int32 t) {
	int32 score = 0, leftRange = 5, rightRange = 5;
	for (; leftRange > 0 && line[leftRange - 1] == t; --leftRange);
	for (; rightRange < 11 && line[rightRange + 1] == t; ++rightRange);
	switch (rightRange - leftRange + 1) {
	default:
		return 2000000;
	case 4:
		if (line[leftRange - 1] == 0 && line[rightRange + 1] == 0)
			return 250; //011110
		else if ((line[leftRange - 1] == 0) || (line[rightRange + 1] == 0))
			return 75; //011112|211110
		break;
	case 3:
		if (line[leftRange - 1] == 0 && line[rightRange + 1] == 0) {
			if ((line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) &&
				(line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1))
				return 15; //2011102
			else if ((line[leftRange - 2] == 0 && line[rightRange + 2] == 0) ||
				((line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) && line[rightRange + 2] == 0) ||
				((line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1) && line[leftRange - 2] == 0))
				return 65; //0011100|2011100|0011102
		}
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && (line[rightRange + 1] == 3 - t || line[rightRange + 1] == -1))
			return 15; //001112
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && (line[leftRange - 1] == 3 - t || line[leftRange - 1] == -1))
			return 15; //211100
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t)
			score += 75; //10111
		if (line[rightRange + 2] == t && line[rightRange + 1] == 0)
			score += 75; //11101
		break;
	case 2:
		if ((line[leftRange - 1] == 3 - t || line[leftRange - 1] == -1) && line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == 0)
			return 15; //211010
		if ((line[rightRange + 1] == 3 - t || line[rightRange + 1] == -1) && line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == 0)
			return 15; //010112
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[rightRange + 1] == 0 && line[rightRange + 2] == 0)
			return 15; //001100
		else if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0) {
			if (line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1) {
				if (line[leftRange - 3] == 0)
					return 15; //0001102
				else if (line[leftRange - 3] == 3 - t || line[leftRange - 3] == -1)
					return 2; //2001102
			}
		} else if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0) {
			if (line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) {
				if (line[rightRange + 3] == 0)
					return 15; //2011000
				else if (line[rightRange + 3] == 3 - t || line[rightRange + 3] == -1)
					return 2; //2011002
			}
		}
		if ((line[leftRange - 1] != 0 && line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && line[rightRange + 3] == 0) ||
			(line[rightRange + 1] != 0 && line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[leftRange - 3] == 0))
			return 2; //211000|000112
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == t)
			score += 75; //11011
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == t)
			score += 75; //11011
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && (line[leftRange - 3] == 3 - t || line[leftRange - 3] == -1) && line[rightRange + 1] == 0)
			score += 15; //210110
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && (line[rightRange + 3] == 3 - t || line[rightRange + 3] == -1) && line[leftRange - 1] == 0)
			score += 15; //011012
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == 0 && line[rightRange + 1] == 0)
			score += 65; //010110
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == 0 && line[leftRange - 1] == 0)
			score += 65; //011010
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[leftRange - 3] == t)
			score += 15; //10011
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && line[rightRange + 3] == t)
			score += 15; //11001
		break;
	case 1:
		if (line[4] == 0 && line[3] == t && line[2] == t && line[1] == t)
			score += 75; //11101
		if (line[6] == 0 && line[7] == t && line[8] == t && line[9] == t)
			score += 75; //10111
		if (line[4] == 0 && line[3] == t && line[2] == t) {
			if ((line[1] == 3 - t || line[1] == -1) && line[6] == 0)
				score += 15; //211010
			else if (line[1] == 0 && (line[6] == 3 - t || line[6] == -1))
				return 15; //011012
			else if (line[1] == 0 && line[6] == 0)
				score += 65; //011010
		}
		if (line[6] == 0 && line[7] == t && line[8] == t) {
			if (line[4] == 0 && (line[9] == 3 - t || line[9] == -1))
				score += 15; //010112
			else if ((line[4] == 4 - t || line[4] == -1) && line[9] == 0)
				return 15; //210110
			else if (line[4] == 0 && line[9] == 0)
				score += 65; //010110
		}
		if ((line[4] == 0 && line[3] == 0 && line[2] == 0 && line[1] == t) || (line[6] == 0 && line[7] == 0 && line[8] == 0 && line[9] == t))
			score += 2; //10001
		if (line[4] == 0 && line[3] == 0 && line[2] == t) {
			if (line[1] == 0 && line[6] == 0)
				score += 15; //010010
			else if (line[1] == 0 && (line[6] == 3 - t || line[6] == -1))
				return 2; //010012
			else if ((line[1] == 3 - t || line[1] == -1) && line[6] == 0)
				score += 2; //210010
			else if (line[1] == t)
				score += 15; //11001
		}
		if (line[6] == 0 && line[7] == 0 && line[8] == t) {
			if (line[4] == 0 && line[9] == 0)
				score += 15; //010010
			else if ((line[4] == 3 - t || line[4] == -1) && line[9] == 0)
				return 2; //210010
			else if (line[4] == 0 && (line[9] == 3 - t || line[9] == -1))
				score += 2; //010012
			else if (line[9] == t)
				score += 15; //10011
		}
		if (line[4] == 0 && line[3] == t && line[6] == 0 && line[7] == t)
			return 15; //10101
		if (line[4] == 0 && line[3] == t && line[2] == 0 && line[1] == t)
			score += 15; //10101
		if (line[6] == 0 && line[7] == t && line[8] == 0 && line[9] == t)
			score += 15; //10101
		if (line[4] == 0 && line[3] == t) {
			if (line[2] == 0 && line[6] == 0) {
				if ((line[1] == 3 - t || line[1] == -1) && (line[7] == 3 - t || line[7] == -1))
					return 2; //2010102
				else if (line[7] == 3 - t || line[7] == -1)
					return 15; //0010102
				else
					score += 15; //2010100|0010100
			} else if ((line[2] == 3 - t || line[2] == -1) && line[6] == 0 && line[7] == 0)
				score += 2; //210100
			else if (line[2] == 0 && line[1] == 0 && line[6] != 0)
				return 2; //001012
		}
		if (line[6] == 0 && line[7] == t) {
			if (line[8] == 0 && line[4] == 0) {
				if ((line[3] == 3 - t || line[3] == -1) && (line[9] == 3 - t || line[9] == -1))
					return 2; //2010102
				else if (line[3] == 3 - t || line[3] == -1)
					return 15; //2010100
				else
					score += 15; //0010102|0010100
			} else if ((line[8] == 3 - t || line[8] == -1) && line[4] == 0 && line[3] == 0) {
				score += 2; //001012
			} else if (line[8] == 0 && line[9] == 0 && (line[4] == 3 - t || line[4] == -1))
				return 2; //210100
		}
		break;
	}
	return score;
}

int32 AGobangFunction::AnalysisPoint_My(const vector<int32>& line, int32 t) {
	int32 myScore = 0, leftRange = 5, rightRange = 5;
	for (; line[leftRange - 1] == t; leftRange--);
	for (; line[rightRange + 1] == t; rightRange++);
	switch (rightRange - leftRange + 1) {
	case 5:
		return 100000000;
	case 4:
		if (line[leftRange - 1] == 0 && line[rightRange + 1] == 0)
			return 1000; //011110
		else if ((line[leftRange - 1] == 0) || (line[rightRange + 1] == 0))
			return 300; //011112|211110
		break;
	case 3:
		if (line[leftRange - 1] == 0 && line[rightRange + 1] == 0) {
			if ((line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) &&
				(line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1))
				return 50; //2011102
			else if ((line[leftRange - 2] == 0 && line[rightRange + 2] == 0) ||
				((line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) && line[rightRange + 2] == 0) ||
				((line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1) && line[leftRange - 2] == 0))
				return 200; //0011100|2011100|0011102
		}
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && (line[rightRange + 1] == 3 - t || line[rightRange + 1] == -1))
			return 50; //001112
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && (line[leftRange - 1] == 3 - t || line[leftRange - 1] == -1))
			return 50; //211100
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t)
			myScore += 300; //10111
		if (line[rightRange + 2] == t && line[rightRange + 1] == 0)
			myScore += 300; //11101
		break;
	case 2:
		if ((line[leftRange - 1] == 3 - t || line[leftRange - 1] == -1) && line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == 0)
			return 50; //211010
		if ((line[rightRange + 1] == 3 - t || line[rightRange + 1] == -1) && line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == 0)
			return 50; //010112
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[rightRange + 1] == 0 && line[rightRange + 2] == 0)
			return 30; //001100
		else if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0) {
			if (line[rightRange + 2] == 3 - t || line[rightRange + 2] == -1) {
				if (line[leftRange - 3] == 0)
					return 30; //0001102
				else if (line[leftRange - 3] == 3 - t || line[leftRange - 3] == -1)
					return 5; //2001102
			}
		} else if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0) {
			if (line[leftRange - 2] == 3 - t || line[leftRange - 2] == -1) {
				if (line[rightRange + 3] == 0)
					return 30; //2011000
				else if (line[rightRange + 3] == 3 - t || line[rightRange + 3] == -1)
					return 5; //2011002
			}
		}
		if ((line[leftRange - 1] != 0 && line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && line[rightRange + 3] == 0) ||
			(line[rightRange + 1] != 0 && line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[leftRange - 3] == 0))
			return 5; //211000|000112
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == t)
			myScore += 300; //11011
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == t)
			myScore += 300; //11011
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && (line[leftRange - 3] == 3 - t || line[leftRange - 3] == -1) && line[rightRange + 1] == 0)
			myScore += 50; //210110
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && (line[rightRange + 3] == 3 - t || line[rightRange + 3] == -1) && line[leftRange - 1] == 0)
			myScore += 50; //011012
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == t && line[leftRange - 3] == 0 && line[rightRange + 1] == 0)
			myScore += 200; //010110
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == t && line[rightRange + 3] == 0 && line[leftRange - 1] == 0)
			myScore += 200; //011010
		if (line[leftRange - 1] == 0 && line[leftRange - 2] == 0 && line[leftRange - 3] == t)
			myScore += 50; //10011
		if (line[rightRange + 1] == 0 && line[rightRange + 2] == 0 && line[rightRange + 3] == t)
			myScore += 50; //11001
		break;
	case 1:
		if (line[4] == 0 && line[3] == t && line[2] == t && line[1] == t)
			myScore += 300; //11101
		if (line[6] == 0 && line[7] == t && line[8] == t && line[9] == t)
			myScore += 300; //10111
		if (line[4] == 0 && line[3] == t && line[2] == t) {
			if ((line[1] == 3 - t || line[1] == -1) && line[6] == 0)
				myScore += 50; //211010
			else if (line[1] == 0 && (line[6] == 3 - t || line[6] == -1))
				return 50; //011012
			else if (line[1] == 0 && line[6] == 0)
				myScore += 200; //011010
		}
		if (line[6] == 0 && line[7] == t && line[8] == t) {
			if (line[4] == 0 && (line[9] == 3 - t || line[9] == -1))
				myScore += 50; //010112
			else if ((line[4] == 4 - t || line[4] == -1) && line[9] == 0)
				return 50; //210110
			else if (line[4] == 0 && line[9] == 0)
				myScore += 200; //010110
		}
		if ((line[4] == 0 && line[3] == 0 && line[2] == 0 && line[1] == t) || (line[6] == 0 && line[7] == 0 && line[8] == 0 && line[9] == t))
			myScore += 5; //10001
		if (line[4] == 0 && line[3] == 0 && line[2] == t) {
			if (line[1] == 0 && line[6] == 0)
				myScore += 30; //010010
			else if (line[1] == 0 && (line[6] == 3 - t || line[6] == -1))
				return 5; //010012
			else if ((line[1] == 3 - t || line[1] == -1) && line[6] == 0)
				myScore += 5; //210010
			else if (line[1] == t)
				myScore += 50; //11001
		}
		if (line[6] == 0 && line[7] == 0 && line[8] == t) {
			if (line[4] == 0 && line[9] == 0)
				myScore += 30; //010010
			else if ((line[4] == 3 - t || line[4] == -1) && line[9] == 0)
				return 5; //210010
			else if (line[4] == 0 && (line[9] == 3 - t || line[9] == -1))
				myScore += 5; //010012
			else if (line[9] == t)
				myScore += 50; //10011
		}
		if (line[4] == 0 && line[3] == t && line[6] == 0 && line[7] == t)
			return 50; //10101
		if (line[4] == 0 && line[3] == t && line[2] == 0 && line[1] == t)
			myScore += 50; //10101
		if (line[6] == 0 && line[7] == t && line[8] == 0 && line[9] == t)
			myScore += 50; //10101
		if (line[4] == 0 && line[3] == t) {
			if (line[2] == 0 && line[6] == 0) {
				if ((line[1] == 3 - t || line[1] == -1) && (line[7] == 3 - t || line[7] == -1))
					return 5; //2010102
				else if (line[7] == 3 - t || line[7] == -1)
					return 30; //0010102
				else
					myScore += 30; //2010100|0010100
			} else if ((line[2] == 3 - t || line[2] == -1) && line[6] == 0 && line[7] == 0)
				myScore += 5; //210100
			else if (line[2] == 0 && line[1] == 0 && line[6] != 0)
				return 5; //001012
		}
		if (line[6] == 0 && line[7] == t) {
			if (line[8] == 0 && line[4] == 0) {
				if ((line[3] == 3 - t || line[3] == -1) && (line[9] == 3 - t || line[9] == -1))
					return 5; //2010102
				else if (line[3] == 3 - t || line[3] == -1)
					return 30; //2010100
				else
					myScore += 30; //0010102|0010100
			} else if ((line[8] == 3 - t || line[8] == -1) && line[4] == 0 && line[3] == 0) {
				myScore += 5; //001012
			} else if (line[8] == 0 && line[9] == 0 && (line[4] == 3 - t || line[4] == -1))
				return 5; //210100
		}
		break;
	}
	return myScore;
}

int32 AGobangFunction::AnalysisPoint_Adversary(const vector<int32>& line, int32 t) {
	int32 opponentScore = 0, cntLeft = 0, cntRight = 0;
	for (; line[4 - cntLeft] == 3 - t; cntLeft++);
	for (; line[6 + cntRight] == 3 - t; cntRight++);
	switch (cntLeft) {
		// NOLINT(hicpp-multiway-paths-covered)
	case 4:
		if (line[0] == 0)
			opponentScore += 100000000 - 300; //011112
		else
			opponentScore += 100000000; //211112
		break;
	case 3:
		if (line[1] == 0) {
			if (line[0] == 0) {
				if (line[6] == 0)
					opponentScore += 1000 - 50; //0011120
				else if (line[6] == t || line[6] == -1)
					opponentScore += 300 - 50; //0011122
			} else if (line[0] == t || line[0] == -1) {
				if (line[6] == 0)
					opponentScore += 1000; //2011120
				else if (line[6] == t || line[6] == -1)
					opponentScore += 300; //2011122
			}
		} else if (line[1] == t || line[1] == -1) {
			if (line[6] == 0)
				opponentScore += 300; //211120
		}
		break;
	case 2:
		if (line[2] == 0) {
			if (line[1] == 0) {
				if (line[6] == 0) {
					if (line[0] == 0)
						opponentScore += 200 - 5; //0001120
					else if (line[0] == t || line[0] == -1)
						opponentScore += 200; //2001120
				} else if (line[6] == t || line[6] == -1) {
					if (line[0] == 0)
						opponentScore += 200 - 5; //0001122
					else if (line[0] == t || line[0] == -1)
						opponentScore += 50; //2001122
				}
			} else if (line[1] == 3 - t) {
				if (line[0] == t || line[0] == -1)
					opponentScore += 300; //210112
			} else if (line[1] == t || line[1] == -1) {
				if (line[6] == 0) {
					if (line[7] == 0)
						opponentScore += 200; //2011200
					else if (line[7] == t || line[7] == -1)
						opponentScore += 50; //2011202
				}
			}
		} else if (line[2] == t || line[2] == -1) {
			if (line[6] == 0 && line[7] == 0)
				opponentScore += 50; //211200
		}
		break;
	case 1:
		if (line[3] == 0) {
			if (line[2] == 0) {
				if (line[1] == 3 - t) {
					if (line[0] == 0)
						opponentScore += 50 - 5; //010012
					else
						opponentScore += 50; //210012
				} else if (line[1] == t || line[1] == -1) {
					if (line[6] == 0) {
						if (line[7] == 0)
							opponentScore += 30; //2001200
						else if (line[7] == t || line[7] == -1)
							opponentScore += 5; //2001202
					}
				} else if (line[1] == 0) {
					if (line[6] == 0)
						opponentScore += 30; //000120
					else if (line[6] == t || line[6] == -1)
						opponentScore += 5; //000122
				}
			} else if (line[2] == 3 - t) {
				if (line[1] == 0) {
					if (line[6] == 0) {
						if (line[0] == 0)
							opponentScore += 200 - 5; //0010120
						else
							opponentScore += 200; //2010120
					} else if (line[6] == t || line[6] == -1) {
						if (line[0] == 0)
							opponentScore += 50 - 5; //0010122
						else
							opponentScore += 50; //2010122
					}
				} else if (line[1] == 3 - t) {
					if (line[0] == t || line[0] == -1)
						opponentScore += 300; //211012
				} else if (line[1] == t || line[1] == -1) {
					if (line[6] == 0)
						opponentScore += 50; //210120
				}
			} else if (line[2] == t || line[2] == -1) {
				if (line[6] == 0 && line[7] == 0 && line[8] == 0)
					opponentScore += 30; //2012000
				else if (line[6] == 0 && line[7] == 0 && (line[8] == t || line[8] == -1))
					opponentScore += 5; //2012002
			}
		} else if (line[3] == t || line[3] == -1) {
			if (line[6] == 0 && line[7] == 0 && line[8] == 0)
				opponentScore += 5; //212000
		}
		break;
	case 0:
		if (line[4] == 0) {
			if (line[3] == 0) {
				if (line[2] == 0) {
					if (line[1] == 3 - t)
						opponentScore += 5; //10002
				} else if (line[2] == 3 - t) {
					if (line[1] == 0) {
						if (line[6] == 0)
							opponentScore += 30; //010020
						else if (line[6] == t || line[6] == -1)
							opponentScore += 5; //010022
					} else if (line[1] == 3 - t) {
						if (line[0] == 0)
							opponentScore += 50 - 5; //011002
						else
							opponentScore += 50; //211002
					} else if ((line[1] == t || line[1] == -1) && line[6] == 0)
						opponentScore += 5; //210020
				}
			} else if (line[3] == 3 - t) {
				if (line[2] == 0) {
					if (line[1] == 0) {
						if (line[6] == 0)
							opponentScore += 30; //001020
						else if (line[6] == t || line[6] == -1)
							opponentScore += 5; //001022
					} else if (line[1] == 3 - t) {
						if (line[0] == t || line[0] == -1)
							opponentScore += 50; //210102
						else if (line[0] == 0)
							opponentScore += 50 - 5; //010102
					} else if (line[1] == t || line[1] == -1) {
						if (line[6] == 0) {
							if (line[7] == 0)
								opponentScore += 30; //2010200
							else if (line[7] == t || line[7] == -1)
								opponentScore += 5; //2010202
						}
					}
				} else if (line[2] == 3 - t) {
					if (line[1] == 0) {
						if (line[0] == 0) {
							if (line[6] == 0)
								opponentScore += 200 - 5; //0011020
							else if (line[6] == t || line[6] == -1) {
								opponentScore += 50 - 5; //0011022
							}
						} else if (line[0] == t || line[0] == -1) {
							if (line[6] == 0)
								opponentScore += 200; //2011020
							else if (line[6] == t || line[6] == -1)
								opponentScore += 50; //2011022
						}
					} else if (line[1] == 3 - t) {
						if (line[0] == t || line[0] == -1)
							opponentScore += 300; //211102
					} else if (line[1] == t || line[1] == -1) {
						if (line[6] == 0)
							opponentScore += 50; //211020
					}
				} else if (line[2] == t || line[2] == -1) {
					if (line[6] == 0 && line[7] == 0)
						opponentScore += 5; //210200
				}
			}
		}
		break;
	}
	switch (cntRight) {
		// NOLINT(hicpp-multiway-paths-covered)
	case 4:
		if (line[10] == 0)
			opponentScore += 10000000 - 300; //211110
		else
			opponentScore += 10000000; //211112
		break;
	case 3:
		if (line[9] == 0) {
			if (line[10] == 0) {
				if (line[4] == 0)
					opponentScore += 1000 - 50; //0211100
				else if (line[4] == t || line[4] == -1)
					opponentScore += 300 - 50; //2211100
			} else if (line[10] == t || line[10] == -1) {
				if (line[4] == 0)
					opponentScore += 1000; //0211102
				else if (line[4] == t || line[4] == -1)
					opponentScore += 300; //2211102
			}
		} else if (line[9] == t || line[9] == -1) {
			if (line[4] == 0)
				opponentScore += 300; //211120
		}
		break;
	case 2:
		if (line[8] == 0) {
			if (line[9] == 0) {
				if (line[4] == 0) {
					if (line[10] == 0)
						opponentScore += 200 - 5; //0211000
					else if (line[10] == t || line[10] == -1)
						opponentScore += 200; //0211002
				} else if (line[4] == t || line[4] == -1) {
					if (line[10] == 0)
						opponentScore += 200 - 5; //2211000
					else if (line[10] == t || line[10] == -1)
						opponentScore += 50; //2211002
				}
			} else if (line[9] == 3 - t) {
				if (line[10] == t || line[10] == -1)
					opponentScore += 300; //211012
			} else if (line[9] == t || line[9] == -1) {
				if (line[4] == 0) {
					if (line[3] == 0)
						opponentScore += 200; //0021102
					else if (line[3] == t || line[3] == -1)
						opponentScore += 50; //2021102
				}
			}
		} else if (line[8] == t || line[8] == -1) {
			if (line[4] == 0 && line[3] == 0)
				opponentScore += 50; //002112
		}
		break;
	case 1:
		if (line[7] == 0) {
			if (line[8] == 0) {
				if (line[9] == 3 - t) {
					if (line[10] == 0)
						opponentScore += 50 - 5; //210010
					else
						opponentScore += 50; //210012
				} else if (line[9] == t || line[9] == -1) {
					if (line[4] == 0) {
						if (line[3] == 0)
							opponentScore += 30; //0021002
						else if (line[3] == t || line[3] == -1)
							opponentScore += 5; //2021002
					}
				} else if (line[9] == 0) {
					if (line[4] == 0)
						opponentScore += 30; //021000
					else if (line[4] == t || line[4] == -1)
						opponentScore += 5; //221000
				}
			} else if (line[8] == 3 - t) {
				if (line[9] == 0) {
					if (line[4] == 0) {
						if (line[10] == 0)
							opponentScore += 200 - 5; //0210100
						else
							opponentScore += 200; //0210102
					} else if (line[4] == t || line[4] == -1) {
						if (line[10] == 0)
							opponentScore += 50 - 5; //2210100
						else
							opponentScore += 50; //2210102
					}
				} else if (line[9] == 3 - t) {
					if (line[10] == t || line[10] == -1)
						opponentScore += 300; //210112
				} else if (line[9] == t || line[9] == -1) {
					if (line[4] == 0)
						opponentScore += 50; //021012
				}
			} else if (line[8] == t || line[8] == -1) {
				if (line[4] == 0 && line[3] == 0 && line[2] == 0)
					opponentScore += 30; //0002102
				else if (line[4] == 0 && line[3] == 0 && (line[2] == t || line[2] == -1))
					opponentScore += 5; //2002102
			}
		} else if (line[7] == t || line[7] == -1) {
			if (line[4] == 0 && line[3] == 0 && line[2] == 0)
				opponentScore += 5; //000212
		}
		break;
	case 0:
		if (line[6] == 0) {
			if (line[7] == 0) {
				if (line[8] == 0) {
					if (line[9] == 3 - t)
						opponentScore += 5; //20001
				} else if (line[8] == 3 - t) {
					if (line[9] == 0) {
						if (line[4] == 0)
							opponentScore += 30; //020010
						else if (line[4] == t || line[4] == -1)
							opponentScore += 5; //220010
					} else if (line[9] == 3 - t) {
						if (line[10] == 0)
							opponentScore += 50 - 5; //200110
						else
							opponentScore += 50; //200112
					} else if ((line[9] == t || line[9] == -1) && line[4] == 0)
						opponentScore += 5; //020012
				}
			} else if (line[7] == 3 - t) {
				if (line[8] == 0) {
					if (line[9] == 0) {
						if (line[4] == 0)
							opponentScore += 30; //020100
						else if (line[4] == t || line[4] == -1)
							opponentScore += 5; //220100
					} else if (line[9] == 3 - t) {
						if (line[10] == t || line[10] == -1)
							opponentScore += 50; //201012
						else if (line[10] == 0)
							opponentScore += 50 - 5; //201010
					} else if (line[9] == t || line[9] == -1) {
						if (line[4] == 0) {
							if (line[3] == 0)
								opponentScore += 30; //0020102
							else if (line[3] == t || line[3] == -1)
								opponentScore += 5; //2020102
						}
					}
				} else if (line[8] == 3 - t) {
					if (line[9] == 0) {
						if (line[10] == 0) {
							if (line[4] == 0)
								opponentScore += 200 - 5; //0201100
							else if (line[4] == t || line[4] == -1) {
								opponentScore += 50 - 5; //2201100
							}
						} else if (line[10] == t || line[10] == -1) {
							if (line[4] == 0)
								opponentScore += 200; //0201102
							else if (line[4] == t || line[4] == -1)
								opponentScore += 50; //2201102
						}
					} else if (line[9] == 3 - t) {
						if (line[10] == t || line[10] == -1)
							opponentScore += 300; //201112
					} else if (line[9] == t || line[9] == -1) {
						if (line[4] == 0)
							opponentScore += 50; //020112
					}
				} else if (line[8] == t || line[8] == -1) {
					if (line[4] == 0 && line[3] == 0)
						opponentScore += 5; //002012
				}
			}
		}
		break;
	}
	if (cntLeft == 1 && cntRight == 3) {
		if (line[9] == 0 && line[10] == 0)
			return opponentScore + 10000000 - 50; //1211100
		else
			return opponentScore + 10000000; //12111
	}
	if (cntLeft == 3 && cntRight == 1) {
		if (line[1] == 0 && line[0] == 0)
			return opponentScore + 10000000 - 50; //0011121
		else
			return opponentScore + 10000000; //11121
	}
	if (cntLeft == 2 && cntRight == 2) {
		if (line[2] == 0 && line[1] == 0 && line[0] == 0 && line[8] == 0 && line[9] == 0 && line[10] == 0)
			return opponentScore + 10000000 - 2 * 5; //00011211000
		else if ((line[2] == 0 && line[1] == 0 && line[0] == 0) || (line[8] == 0 && line[9] == 0 && line[10] == 0))
			return opponentScore + 10000000 - 5; //00011211|11211000
		else
			return opponentScore + 10000000; //11211
	}
	if (cntLeft == 1 && cntRight == 2) {
		if (line[3] == 0 && line[8] == 0) {
			if (line[9] == 0 && line[10] == 0)
				return opponentScore + 1000 - 5; //01211000
			else
				return opponentScore + 1000; //012110
		} else if (line[8] == 0) {
			if (line[9] == 0 && line[10] == 0)
				return opponentScore + 300 - 5; //21211000
			else
				return opponentScore + 300; //212110
		} else if (line[3] == 0)
			return opponentScore + 300; //012112
	}
	if (cntLeft == 2 && cntRight == 1) {
		if (line[7] == 0 && line[2] == 0) {
			if (line[1] == 0 && line[0] == 0)
				return opponentScore + 1000 - 5; //00011210
			else
				return opponentScore + 1000; //011210
		} else if (line[2] == 0) {
			if (line[1] == 0 && line[0] == 0)
				return opponentScore + 300 - 5; //00011212
			else
				return opponentScore + 300; //011212
		} else if (line[7] == 0)
			return opponentScore + 300; //211210
	}
	if (cntLeft == 0 && cntRight == 2 && line[4] == 0) {
		if (line[3] == 3 - t) {
			if (line[8] == 0 && line[9] == 0 && line[10] == 0)
				return opponentScore + 300 - 5; //10211000
			else
				return opponentScore + 300; //10211
		}
	}
	if (cntLeft == 2 && cntRight == 0 && line[6] == 0) {
		if (line[7] == 3 - t) {
			if (line[2] == 0 && line[1] == 0 && line[0] == 0)
				return opponentScore + 300 - 5; //00011201
			else
				return opponentScore + 300; //11201
		}
	}
	if (cntLeft == 1 && cntRight == 1) {
		if ((line[3] == t || line[3] == -1) && line[7] == 0 && line[8] == 0)
			return opponentScore + 50; //212100
		if ((line[7] == t || line[7] == -1) && line[3] == 0 && line[2] == 0)
			return opponentScore + 50; //001212
		if (line[3] == 0 && line[2] == 3 - t && line[7] == 0 && line[8] == 3 - t) {
			if (line[1] == 0 && line[0] == 0 && line[9] == 0 && line[10] == 0)
				return opponentScore + 2 * 300 - 2 * 5; //00101210100
			else if ((line[1] == 0 && line[0] == 0) || (line[9] == 0 && line[10] == 0))
				return opponentScore + 2 * 300 - 5; //001012101||101210100
			else
				return opponentScore + 2 * 300; //1012101
		}
		if (line[3] == 0 && line[2] == 3 - t) {
			if (line[1] == 0 && line[0] == 0)
				return opponentScore + 300 - 5; //0010121
			else
				return opponentScore + 300; //10121
		}
		if (line[7] == 0 && line[8] == 3 - t) {
			if (line[9] == 0 && line[10] == 0)
				return opponentScore + 300 - 5; //1210100
			else
				return opponentScore + 300; //12101
		}
		if (line[3] == 0 && line[7] == 0) {
			if ((line[2] == t || line[2] == -1) && (line[8] == t || line[8] == -1))
				return opponentScore + 50; //2012102
			else if (((line[2] == t || line[2] == -1) && line[8] == 0) || ((line[8] == t || line[8] == -1) && line[2] == 0) || (line[2] == 0 && line[8] == 0))
				return opponentScore + 200; //2012100|0012102|0012100
		}
	}
	if (cntLeft == 0 && cntRight == 1 && line[4] == 0) {
		if (line[3] == 0 && line[2] == 3 - t)
			return opponentScore + 50; //10021
		if (line[3] == 3 - t) {
			if (line[2] == 0 && line[7] == 0)
				return opponentScore + 200; //010210
			else if (line[2] == 3 - t)
				return opponentScore + 300; //11021
			else if (((line[2] == t || line[2] == -1) && line[7] == 0) || (line[2] == 0 && (line[7] == t || line[7] == -1)))
				return opponentScore + 50; //210210|010212
		}
	}
	if (cntLeft == 1 && cntRight == 0 && line[6] == 0) {
		if (line[7] == 0 && line[8] == 3 - t)
			return opponentScore + 50; //12001
		if (line[7] == 3 - t) {
			if (line[8] == 0 && line[3] == 0)
				return opponentScore + 200; //012010
			else if (line[8] == 3 - t)
				return opponentScore + 300; //12011
			else if (((line[8] == t || line[8] == -1) && line[3] == 0) || (line[8] == 0 && (line[3] == t || line[3] == -1)))
				return opponentScore + 50; //012012|212010
		}
	}
	if (cntLeft == 0 && cntRight == 0 && line[4] == 0 && line[6] == 0) {
		if (line[3] == 3 - t && line[7] == 3 - t)
			return opponentScore + 50; //10201
	}
	return opponentScore;
}

void AGobangFunction::BeginPlay() {
	Super::BeginPlay();

}

void AGobangFunction::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

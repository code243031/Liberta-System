#pragma once
#include <mysql.h>
#include <algorithm> // sort()
#include <vector>	 // vector
#include <iostream>
#include <string>
#include <tuple>	// tuple
using namespace std;

void finish_with_error(MYSQL* con);

// void descending_sort(vector<tuple<double, double, string>>& Patient);

bool sortdesc(const tuple<double, double, string>& P1,
    const tuple<double, double, string>& P2);
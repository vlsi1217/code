#include <cstdio>
#include <cstring>
#include <climits>
#include <iostream>
using namespace std;

const int MAXN = 105;
const int INF = INT_MAX / 2;

int c[MAXN][MAXN], pre[MAXN], d[MAXN];
int q[MAXN];

int maxflow(int n, int s, int t) {
	int *front = q, *tail = q;
	memset(d, -1, sizeof(d));
	*tail++ = t;
	d[t] = 0;
	while (front < tail) {
		int v = *front++;
		for (int u = 0; u < n; ++u) {
			if (d[u] == -1 && c[u][v] > 0) {
				d[u] = d[v] + 1;
				*tail++ = u;
			}
		}
	}
	int flow = 0, u = s;
	while (d[s] < n) {
		bool flag = false;
		for (int v = 0; v < n; ++v) {
			if (c[u][v] > 0 && d[u] == d[v] + 1) {
				flag = true;
				pre[v] = u;
				u = v;
				break;
			}
		}
		if (flag) {
			if (u == t) {
				int delta = INF;
				for (; u != s; u = pre[u]) {
					if (c[pre[u]][u] < delta) delta = c[pre[u]][u];
				}
				for (u = t; u != s; u = pre[u]) {
					c[pre[u]][u] -= delta;
					c[u][pre[u]] += delta;
				}
				flow += delta;
				u = s;
			}
		} else {
			d[u] = n + 1;
			for (int i = 0; i < n; ++i) {
				if (c[u][i] > 0 && d[u] > d[i] + 1) d[u] = d[i] + 1;
			}
			if (u != s) u = pre[u];
		}
	}
	return flow;
}

int pig[1005];
bool vis[1005], need[MAXN][1005];

int main()
{
	int n, m;
	while (EOF != scanf("%d%d", &m, &n)) {
		int src = 0, dst = n + 1;
		memset(c, 0, sizeof(c));
		memset(need, 0, sizeof(need));
		for (int i = 1; i <= m; ++i) {
			vis[i] = false;
			scanf("%d", &pig[i]);
		}
		for (int i = 1; i <= n; ++i) {
			int a, b, r = 0;
			scanf("%d", &a);
			for (int j = 0; j < a; ++j) {
				int p;
				scanf("%d", &p);
				need[i][p] = true;
				if (!vis[p]) {
					r += pig[p];
					vis[p] = true;
				}
			}
			scanf("%d", &b);
			c[src][i] = r;
			c[i][dst] = b;
		}
		for (int i = 1; i < n; ++i) {
			for (int j = i+1; j <= n; ++j) {
				for (int k = 1; k <= m; ++k) {
					if (need[i][k] && need[j][k]) {
						c[i][j] = INF;
					}
				}
			}
		}
		printf("%d\n", maxflow(n+2, src, dst));
	}
	return 0;
}


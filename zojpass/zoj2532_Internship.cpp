/* ZOJ 2532 Internship
 * 最大流最小割
 * 只升级一条边则能提高带宽，问有多少条这样的边，请一一列举出来。
 * */

#include <cstdio>
#include <cstring>
#include <climits>

const int INF = INT_MAX / 3;
const int MAXN = 205;
const int MAXM = 2005;

struct Edge {
	int u, v;
	int c;
	int next;
	Edge() {}
	Edge(int tu, int tv, int tc, int tn) : u(tu), v(tv), c(tc), next(tn) {}
};

Edge E[MAXM * 3];
int nE, head[MAXN], cnt[MAXN], que[MAXN], d[MAXN], low[MAXN], cur[MAXN];

void addEdge(int u, int v, int c, int rc = 0) {
	E[nE]= Edge(u, v, c, head[u]);
	head[u] = nE++;
	E[nE]= Edge(v, u, rc, head[v]);
	head[v] = nE++;
}

int maxflow(int n, int source, int sink) {
	int *fr = que, *ta = que;;
	for (int i = 0; i < n; ++i) d[i] = n, cnt[i] = 0;
	cnt[n] = n - 1, cnt[0]++, d[sink] = 0;
	*ta++ = sink;
	while (fr < ta) {
		int v = *fr++;
		for (int i = head[v]; i != -1; i = E[i].next) {
			if (d[E[i].v] == n && E[i^1].c > 0) {
				d[E[i].v] = d[v] + 1;
				cnt[n]--;
				cnt[d[E[i].v]]++;
				*ta++ = E[i].v;
			}
		}
	}
	int flow = 0, u = source, top = 0;
	low[0] = INF;
	for (int i = 0; i < n; ++i) cur[i] = head[i];
	while (d[source] < n) {
		int &i = cur[u];
		for (; i != -1; i = E[i].next) {
			if (E[i].c > 0 && d[u] == d[E[i].v] + 1) {
				low[top+1] = low[top] < E[i].c ? low[top] : E[i].c;
				que[top+1] = i;
				++top;
				u = E[i].v;
				break;
			}
		}
		if (i != -1) {
			if (u == sink) {
				int delta = low[top];
				for (int p = 1, i; p <= top; ++p) {
					i = que[p];
					E[i].c -= delta;
					E[i^1].c += delta;
				}
				flow += delta;
				u = source;
				low[0] = INF;
				top = 0;
			}
		} else {
			int old_du = d[u];
			cnt[old_du]--;
			d[u] = n - 1;
			for (int i = head[u]; i != -1; i = E[i].next) {
				if (E[i].c > 0 && d[u] > d[E[i].v]) d[u] = d[E[i].v];
			}
			cnt[++d[u]]++;
			if (d[u] < n) cur[u] = head[u];
			if (u != source) {
				u = E[que[top]].u;
				--top;
			}
			if (cnt[old_du] == 0) break;
		}
	}
	return flow;
}
Edge ET[MAXM * 3];
int nET, VT[MAXN];

void addEdgeT(int u, int v, int c, int rc = 0) {
	ET[nET]= Edge(u, v, c, VT[u]);
	VT[u] = nET++;
	ET[nET]= Edge(v, u, rc, VT[v]);
	VT[v] = nET++;
}

int sig[MAXN];

void bfs(int n, int V[], Edge E[], int source, int sig[], int val) {
	int *fr = que, *ta = que;
	*ta++ = source;
	sig[source] = val;
	while (fr < ta) {
		int u = *fr++;
		for (int i = V[u]; i != -1; i = E[i].next) {
			if (i&1) continue;
			int v = E[i].v;
			if (sig[v]) continue;
			if (E[i].c > 0) {
				sig[v] = val;
				*ta++ = v;
			}
		}
	}
}

int main()
{
	int n, m, l, sink, source;
	while (EOF != scanf("%d%d%d", &n, &m, &l)) {
		if (n == 0 && m == 0 && l == 0) break;
		source = n + m + 1, sink = 0;
		memset(head, -1, sizeof(head[0])*(n+m+2));
		nE = 0;
		for (int i = 0, a, b, c; i < l; ++i) {
			scanf("%d%d%d", &a, &b, &c);
			addEdge(a, b, c);
		}
		for (int i = 1; i <= n; ++i) {
			addEdge(source, i, INF);
		}
		int flow = maxflow(n+m+2, source, sink);

		memset(VT, -1, sizeof(VT));
		nET = nE;
		for (int i = 0, t; i < nE; i += 2) {
			int u = E[i].v, v = E[i].u, c = E[i].c;
			if (c > 0) addEdgeT(u, v, c);
		}
		memset(sig, 0, sizeof(sig));
		bfs(n+m+2, head, E, source, sig, 1);
		bfs(n+m+2, VT, ET, sink, sig, 2);
		bool first = true;
		for (int i = 0, u, v; i < nE; i += 2) {
			if (E[i].c) continue;
			u = E[i].u, v = E[i].v;
			if (sig[u] == 1 && sig[v] == 2) {
				if (first)  first = false;
				else putchar(' ');
				printf("%d", i / 2 + 1);
			}
		}
		puts("");
	}
	return 0;
}


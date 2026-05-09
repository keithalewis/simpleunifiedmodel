# Simple Unified Model.

Every arbitrage-free model of prices $X_t$ and cash flows
$C_t$ indexed by intruments $I$ has the form
$$
X_t D_t = X_0 M_t - \sum_{t < s \le u} C_s D_s
$$
where $M_t$ is a martingale measure indexed by $I$$
and $D_t$ is a positive measure.

If a cash-settled derivative pays $\hat{A_j}$ at
increasing stopping times $\hat{\tau}_t$ then its value
at time $t$ is
$$
V_t D_t = \sum_{\hat{\tau}_j > t} \hat{A_j} D_{\hat{\tau}_j.
$$
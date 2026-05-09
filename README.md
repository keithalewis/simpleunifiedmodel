# Simple Unified Model.

Every arbitrage-free model of prices $(X_t)_{t\in T}$ and cash flows
$(C_t)_{t\in T}$ indexed by intruments $I$ has the form

$$
X_t D_t = X_0 M_t - \sum_{s\le t} C_s D_s
$$

where $(M_t)$ is a martingale measure indexed by $I$
and $(D_t)$ are positive measures.

Partial information at time $t$ is modeled by a partition
$𝓐_t$ of the sample space $\Omega$. 


This implies

$$
X_t D_t = (X_u D_u + \sum_{t < s \le u} C_u D_u)|_{𝓐_t}\quad\text{(1)}
$$

In the case of zero cash flows this says $(X_t D_t)$
is a martingale measure (Black-Scholes/Merton).
If $X_u D_u$ goes to zero as
$u$ goes to infinity, this says price is the discounted
value of future cash flows (Graham-Todd).

Given a trading strategy purchasing $(\Gamma_j)$ shares
at increasing stopping times $\tau_j$ the position
at time $t$ is $\Delta_t = \sum_{\tau_j < t} \Gamma_j$.
The value (or mark-to-market) of the
strategy at time $t$ is its liquidation value
at current market prices

$$
V_t = (\Delta_t + \Gamma_t)\cdot X_t
$$

and the amount associated with trading is cash flows
proportional to position minus the cost of the trade just
executed.

$$
A_t = \Delta_t\cdot C_t - \Gamma_t\cdot X_t.
$$

A consequence of these definitions is

$$
V_t D_t = (V_u D_u + \sum_{t < s \le u} A_u D_u)|_{𝓐_t}\quad\text{(2)}
$$

Note how price in (1) correpond to value in (2)
and cash flows in (1) correpond amount in (2)

**Trading strategies create synthetic market instruments.**

If a cash-settled derivative pays $\hat{A_j}$ at
increasing stopping times $\hat{\tau}_j$ then its value
at time $t$ is

$$
V_t D_t = \sum_{\hat{\tau}_j > t} \hat{A_j} D_{\hat{\tau}_j}.
$$

We assume prices, cash flows, and shares are bounded and
the measures are finitely additive. Recall the vector
dual space of bounded functions on $S$, $B(S)$ is isometrically
isomorphic to the vector space of finitely additive measures
on $S$, $ba(S)$, with the dual pairing 

$$
\langle f,\mu\rangle = \int_S f\,d\mu.
$$

If $S$ is finite then

$$
\langle f,\mu\rangle = \sum_{s\in S} f(s)\,\mu(\{s\}).
$$

If $g\in B(S)$ then $M_g\colon B(S)\to B(S)$ given by
multiplication 
$M_g f = fg$ is bounded. 
Its adjoint $M_g^*\colon ba(S)\to ba(S)$
is used to define muliplication of a measure by a bounded
function $g\mu = M_g^*\mu$.

We first consider the problem of a European derivative paying
$\hat{A}$ at non random time $\hat{\tau}$.


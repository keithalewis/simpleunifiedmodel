# Simple Unified Model.

Let $T$ be a set of trading times, $I$ the set of all instruments,
and $\Omega$ the set of all possible outcomes.
Let $(𝒜_t)_{t\in T})$ be partitions of $\Omega$ 
representing information. For $t < u$ the functions $e_{tu}$
takes atoms of $𝒜_t$ to a finite collections of atoms of $𝒜_u$
where $\omega\in A\in 𝒜_t$ implies $\omega$ meets
some atom of $e_{tu}(A)$

Every arbitrage-free model of prices $(X_t)$ and cash flows
$(C_t)$ for $t\in T$ indexed by instruments $I$ has the form

$$
X_t D_t = X_0 M_t - \sum_{s\le t} C_s D_s
$$

This implies

$$
X_t D_t = (X_u D_u + \sum_{t < s \le u} C_s D_s)|_{𝒜_t}\quad\text{(1)}
$$

In the case of zero cash flows this says $(X_t D_t)$
is a martingale measure (Black-Scholes/Merton).
If $X_u D_u$ goes to zero as
$u$ goes to infinity, this says price is the discounted
value of future cash flows (Graham-Todd).

Given a trading strategy purchasing $(\Gamma_j)$ shares
at increasing stopping times $\tau_j$ the position
at time $t$ is 

$$
\Delta_t = \sum_{\tau_j < t} \Gamma_j.
$$

Note the strict inequality. It takes some time for a trade to settle into a position.
Note also that $\Delta_t + \Gamma_t = \Delta_{t + \epsilon}$
for sufficiently small $\epsilon > 0$.

The value (or mark-to-market) of the
strategy at time $t$ is its liquidation value
at current market prices

$$
V_t = (\Delta_t + \Gamma_t)\cdot X_t
$$

The amount associated with trading is receiving cash flows
proportional to existing position and paying the cost of the trade just
executed.

$$
A_t = \Delta_t\cdot C_t - \Gamma_t\cdot X_t.
$$

A consequence of these definitions is

$$
V_t D_t = (V_u D_u + \sum_{t < s \le u} A_u D_u)|_{𝒜_t}\quad\text{(2)}
$$

Note how price in (1) correpond to value in (2)
and cash flows in (1) correpond amount in (2)

**Trading strategies create synthetic market instruments.**

If a cash-settled derivative pays $\hat{A_j}$ at
increasing stopping times $\hat{\tau}_j$ then its value
at time $t$ is determined by

$$
V_t D_t = (\sum_{\hat{\tau}_j > t} \hat{A_j} D_{\hat{\tau}_j})|_{𝒜_t}.\quad\text{(3)}
$$

_if_ we can find a trading strategy with $A_t - \hat{A}_j 1(t = \hat{\tau}_j) = 0$
for all $t\in T$. [^1]

[^1]: In general this is not possible. An area for future research
is to find trading strategies that make the difference
white noise with miniumum variance.

Since $V_t = (\Delta_t + \Gamma_t)\cdot X_t$ we have the
Frechet derivative $D_{X_t} V_t \Delta_t + \Gamma_t$.
Since the position at time 0 is $\Delta_0 = 0$ this gives us
our first trade $\Gamma_0 D_{X_0} V_0$.
At times greater than 0 we have $\Gamma_t = D_{X_t} V_t - \Delta_t$.
Since $\Delta_t$ is the accumulation of prior tradesd, 
this determined $\Gamma_t$. What this _does not_ determine is
when to trade. If we trade at positive integral multiples of some
fixed $\Delta t$ and let $\Delta t \to 0$ then we get the usual
Black-Scholes/Merton continuous time delta hedging strategy.

## Implementation

We assume prices, cash flows, and shares are bounded and
the measures are finitely additive. Recall the vector
dual space of bounded functions on $S$, $B(S)$, is isometrically
isomorphic to the vector space of finitely additive measures
on $S$, $ba(S)$, with the dual pairing 

$$
\langle f,\mu\rangle = \int_S f d\mu.
$$

If $S$ is finite then

$$
\int_S f d\mu = \sum_{s\in S} f(s) \mu(\{s\}).
$$

If $g\in B(S)$ then $M_g\colon B(S)\to B(S)$ given by
multiplication 
$M_g f = fg$ is bounded. 
Its adjoint $M_g^*\colon ba(S)\to ba(S)$
is used to define muliplication of a measure by a bounded
function $g\mu = M_g^\*\mu$.

Each term in the sum (3) has the form $(f\mu)(A)$ for $A\in 𝒜$
where $f\in B(ℬ)$ and $\mu\in ba(ℬ)$ where $ℬ$ is a refinement of $𝒜$.
We have

$$
\int_A f d\mu = \sum_{B\subseteq A, B\in ℬ} f(B)\mu(B)
$$

### Required Functions

Return all atoms of $ℬ$ contained in $A\in 𝒜$  
$\text{atoms}_ℬ\colon 𝒜\to 2^ℬ$.

The measure $f\mu\in ba(𝒜)$  
$\text{meas}\colon B(ℬ)\times ba(ℬ)\to ba(𝒜)$.

This can be implemented as

$$
\text{meas}(f, \mu)(A) = \sum_{B\in\text{atoms}_ℬ(A)} f(B) \mu(B).
$$

## Binomial Model

The sample space $\Omega = \{0,1\}^𝑵$ is the set of all sequences of 0's and 1's
and $𝑵 = \{0, 1, 2, \ldots\}$. Given $\omega\in\Omega$
define $V_n(\omega) = \sum_{j\le n} \omega_j$. There is a probability
measure on $\Omega$ with $P(V_n = k) = C(n,k)/2^n$ where
$C(n,k) = n!/k!(n - k)!$. Define random walks $W_n = 2V_n - n$.
Note $W_n$ goes from $-n$ to $n$ in steps of size 2 as
$V_n$ goes from $0$ to $n$. We will use the fact $W_n P$ is a martingale measure.

The partition at time $n$ is $𝒜_n = {0,1,\dots,n}$. For $j\in 𝒜_n$
this represents the information $V_n = j$.

If $V_k = i$ then the atoms of $𝒜_n$ given this information
are $V_n = j$ for $j = i, i + 1, \ldots, i + (n - i)$.

If $W_k = i$ then the atoms of $𝒜_n$ given this information
are $W_n = j$ for $j = i - (n - i), i - (n - 1) + 2, \ldots, i + (n - i)$.

## European Option

We first consider the problem of a European derivative paying
$\hat{A}$ at non random time $\hat{\tau}$.


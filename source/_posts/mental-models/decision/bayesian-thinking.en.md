---
title: 'Bayesian Thinking: Update on Evidence, Don''t Bet on Your Gut'
lang: en
permalink: en/mental-models/bayesian-thinking/
alternate: /mental-models/bayesian-thinking/
categories:
  - Mental Models
  - Decision
tags:
  - Statistics
  - Probability
  - Decision
  - Inference
  - Machine Learning
  - Evaluation
  - Uncertainty
  - LLM
  - Forecasting
abbrlink: 66fc1937
date: 2026-07-01 16:00:00
---

> **Bayesian Thinking**
> EN: A belief isn't a yes/no switch; it's a probability you keep updating as evidence arrives.
> 中文：信念不是"信 / 不信"的开关，而是一个随证据不断更新的概率。

Here's something everyone in AI does every day and almost nobody does well: **deciding how much a new piece of evidence should change your mind.**

<!-- more -->

## Why ask this

Let me describe a scene you've probably lived through.

A new model ships, someone runs the eval once, and the score jumps from 71 to 76. Slack lights up—"the new approach works!"—and the PR merges that afternoon. The next day someone reruns it with a different random seed and it's back to 72. Those five points were never capability; they were noise. But they'd already changed the whole team's mind, and changed the code.

The opposite version is just as common. A rarely-seen category in the eval suddenly flashes red, an alert fires, the whole team drops into firefighting mode—and after an hour of digging, it turns out to be one unlucky sample. Nothing broke.

One more: someone runs an impressive demo, drops the screenshot in the channel, and everyone instantly believes "the model can do X." Then it hits real production load and face-plants eight times out of ten.

All three are the same bug: **we're far too quick to let a single observation—one eval, one alert, one demo—slam our belief from "not sure" all the way to "certain."** The better move is almost embarrassingly simple: take a small step, sized to how strong the evidence actually is.

The algorithm for "how big a step" was written down by an English minister more than two centuries ago.

## Where it comes from

Thomas Bayes, an 18th-century English Presbyterian minister who did mathematics on the side, never published the result himself. His friend Richard Price found it in his papers after Bayes died and published it for him in 1763.

The problem he solved, in plain words: **I'm holding a hypothesis, a new piece of evidence just arrived—what should I now set the probability of "this hypothesis is true" to?**

Decades later, Laplace in France rederived the whole thing independently and generalized it—another case of several people walking into the same wall from different directions, which usually means the wall is load-bearing. Funny enough, the rule then stayed philosophically contested for nearly two centuries ("is probability an objective frequency, or your subjective degree of belief?") before quietly becoming one of the foundations of modern machine learning.

## The core intuition

Strip the math off Bayes and you're left with one multiplication:

> new belief ＝ old belief × how convincing this evidence is

Three words:

- **Prior**: before you saw the evidence, how much you already believed the thing.
- **Likelihood (evidence strength)**: if the hypothesis were true, how likely you'd be to see exactly this evidence.
- **Posterior**: your updated belief after the evidence.

Two knobs decide how far you move. **If the claim was very unlikely to begin with (low prior), you need very strong evidence to move it much. If the evidence is weak—you'd see it whether or not the hypothesis is true—it barely moves you at all.**

An eval that swings five points just from a new random seed is the second kind: those five points show up easily under both "the new method works" and "pure luck," so as evidence they carry almost zero strength—and by rights they shouldn't move you.

Keep one key handy, because we'll use it again and again: **a belief is a dial, not a switch.**

{% mermaid %}
graph LR
  A[Prior belief before evidence] --> C[Posterior updated belief]
  B[Strength of new evidence] --> C
  C -.this posterior becomes next prior.-> A
{% endmermaid %}

Notice that dashed line: **this isn't a one-shot deal, it's a loop.** This round's posterior is next round's prior. You never arrive at "I'm certain"; you just keep nudging, one small step at a time. That's exactly the core of the Orient step in the [OODA Loop](/en/mental-models/ooda-loop/)—orienting is, at bottom, updating your posterior with fresh intel.

## A real-world example

There's a classic problem in medical statistics that almost everyone gets wrong the first time.

Say a disease has a base rate of 1 in 1,000. There's a very accurate test: if you have the disease, it comes back positive 99% of the time; if you don't, it falsely reads positive only 1% of the time. You test positive. What's the chance you actually have the disease?

Your gut screams "99%—the test is that good." The real answer: **about 9%.**

Here's the arithmetic. Take 1,000 people: 1 truly has the disease, and the test flags that 1 as positive. Of the other 999 healthy people, the 1% false-positive rate flags about 10 of them positive too. So you get roughly 11 positives, and only 1 is real. One in eleven—about 9%.

That entire 90-point gap comes from one thing your gut skipped: **the disease is rare to begin with.** The denominator is packed with healthy people, so even a tiny 1% false-positive rate produces, in raw numbers, ten times more false alarms than real cases.

So here's a second key, to use alongside the first: **for a rare event, even a very accurate test reading "positive" is probably wrong.**

## Mapping to AI: one positive is mostly noise

AI is Bayesian almost everywhere, because the job is literally "update your beliefs about the world using data." A few of the tightest fits.

**Prior × likelihood is something you already use daily.** Weight decay (L2 regularization), in Bayesian eyes, is just a prior on the weights—"they should be small by default, unless the data strongly objects." Bayesian optimization for hyperparameter search takes it further and turns the idea into an algorithm: it holds a belief about which hyperparameters are promising, updates that belief after each expensive run, and spends the next run where the expected payoff is highest. That's Bayesian thinking written as code: **spend your limited experiments where they'll update your beliefs the most.**

**Rare bugs and flaky eval signals are that disease problem.** Your regression detector flashes red on some category that rarely fails—before you firefight, apply the key: how rare is this failure, and how often does the detector false-alarm? On a rare event, **a "positive" is probably noise, not a real break.** Flaky tests, jittery evals, the occasional spurious alert—all variants of the same problem.

**Why a single eval run should barely move you.** Temperature, sampling, a test set of a few hundred items—all of it gives one run real variance. Those five points show up easily whether the model truly got better or just got lucky; the likelihood ratio sits near 1, so by the formula it earns a tiny update, nothing more. **Weak evidence, small step.** Five hundred examples with a tight confidence interval that reproduces—that's strong evidence, and that's what earns a big revision.

**A demo tells you "possible," not "reliable."** Everyone loves to over-update on one dazzling demo. But a demo is a single sample from a distribution you don't understand (and often a cherry-picked one). It proves the thing can happen; it barely moves your belief about "happens reliably in production."

**Don't call an A/B test on day one either.** Early data is small-sample and wide-interval; your belief in "B beats A" should be a distribution, not a yes/no. Wait for the interval to tighten before you conclude—don't go all-in on weak evidence.

**Calibration is just honestly reporting probabilities.** A well-calibrated model, on the predictions where it claims 70% confidence, should be right about 70% of the time. That's the Bayesian spirit landing in engineering: **report a probability, not a bare flag with no uncertainty attached.**

So why shouldn't those five points have thrilled anyone? One sentence: **weak evidence only buys a small update.** The right move wasn't to flip the flag—it was to nudge the dial one notch toward "the new method might work," and then go earn stronger evidence.

## The engineer's view: swap "I believe / I don't" for "I'm X percent sure"

Enough principle—down to moves you can use tomorrow.

**Attach a probability to every belief, not a flag.** Don't say "the new model is better (true / false)." Say "I'm about 60% the new model is better, and I know what evidence would change that number." The first gets overturned by one jittery run; the second holds up.

**One observation earns one small step.** Want a big revision? Earn it: run the eval N times, report the mean and a confidence interval, and treat any single run as a hint, not a verdict.

**Estimate the base rate before you panic.** Before you treat an alert or a regression as real, run the disease problem: how rare is the thing, how accurate is the detector. On rare events, assume a positive is probably false—then go check.

**Let evidence strength set the size of the update.** Big sample + tight interval + reproducible → revise a lot; one demo → nudge a little. This one rule alone will catch half your bad calls.

**Write down your prior before you look at the data.** Before you start, note what you expect to see, even in one line. Then afterward you can honestly tell the difference between "the data changed me" and "I quietly changed my belief to fit the conclusion I wanted." It's the best antidote to anchoring on your first guess and explaining away everything that disagrees.

**When debugging, don't anchor on the first hypothesis.** Keep two or three hypotheses alive, each with a probability; every test result reweights them, instead of marching down the first guess to the bitter end. That reweighting is exactly what Orient does.

The stance for this whole section in one line: **translating "I believe / I don't" into "I'm X percent sure" is the cheapest and most underrated upgrade in this field.**

## The investment view: good investors are all secretly doing Bayes

Move the lens to investing and Bayes shows up in a new outfit.

A decent investment thesis is a probabilistic bet: I'm 70% this company wins. After that, every earnings report and every data point is evidence that should reweight the number. The bad investor does the opposite—he anchors on the thesis he had at entry, then treats every good sign as confirmation and explains every bad one away. **Falling in love with a position is just handing it a prior that never updates.**

Base rates bite here too: the vast majority of startups fail, so a dazzling pitch (a "positive") landing on the very rare outcome of "unicorn" is probably still noise. Getting starry-eyed over a founder's polished demo and getting thrilled by a jittery eval are the same base-rate error.

Looping back to building AI: when you're sizing up an AI system—or an AI company—to bet on, treat a pretty benchmark or a stunning demo as exactly that, a weak positive on a rare event. **Don't flip the flag; nudge the dial one notch, then earn stronger evidence from your own private evals and from behavior under real load.**

## The life view: strong opinions, weakly held

This reflex doesn't stay in the server room.

You meet someone once, they're rude, and you instantly label them "a jerk." That's over-updating on a single sample (n=1) while ignoring the base rate—everyone has a bad day. Refusing to fly after reading one crash story is the same error: one vivid, frightening data point mistaken for strong evidence.

The healthy Bayesian posture already has a Silicon Valley slogan: **strong opinions, weakly held.** You're allowed a confident view (a posterior built on a lot of evidence), but you stay clear-eyed about exactly what counter-evidence would flip it. Opinionated, not dogmatic.

Back to AI: reading one eval, one demo, one user complaint calls for the same reflex—**a single loud data point is only a weak update.** Ask for the base rate and the sample size before you flip.

## Common misconceptions

**Misconception 1: Bayes is a formula you compute every time.** It isn't. The value is a posture for holding beliefs—probabilities instead of flags, updates sized to evidence, base rates checked first—not actually writing out Bayes' rule. In practice you almost never compute; you just estimate which way to move, and how far.

**Misconception 2: constantly updating means you're spineless, a weathervane. Exactly backwards.** Bayes hands you both things at once: with enough evidence you may hold a strong view, and against strong counter-evidence you must change it. A weathervane updates on the loudest voice; a Bayesian updates on the strongest evidence. It's the opposite of dogma, not the opposite of conviction.

**Misconception 3: the prior is subjective, so Bayes is unreliable and you can set it however you like.** The prior only dominates when evidence is thin; with enough good evidence, two honest people starting from different priors converge to the same posterior. A bad prior isn't fatal—**refusing to update it is.** A prior you never revise is the truly subjective one.

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing the model. Bayesian updating has a few quiet assumptions; break them and it stops working:

**The truth has to be on your list of hypotheses.** Bayes only reshuffles probability among the hypotheses you're already considering. If the real cause isn't on the list, you'll converge—confidently—on "the best-fitting wrong option." What you need then isn't reweighting; it's adding a new hypothesis, which is a different move entirely.

**The evidence has to be genuinely independent.** If your "pile of evidence" all comes from the same broken eval harness or the same contaminated test set, it feels like many independent data points but it's really one. Counting correlated evidence as independent is the number-one cause of being confident and wrong.

**The prior can't be 0 or 1.** Once you set a probability to an absolute 0 or 100%, no evidence can ever move it—dogma is a Bayesian dead end. Which is the method's one demand on you: never round your certainty all the way up to 100%.

The test is those same two keys, run backwards: **"how much should I really change my mind here?"—first ask how strong the evidence is, how rare the event was, and whether these data points actually trace back to the same source.** Weak, common, or correlated → nudge; only strong, independent, and surprising evidence earns a big revision.

## Related models

Bayes isn't an island; it hangs in a web (articles coming as I write them):

- [OODA Loop](/en/mental-models/ooda-loop/): the Orient step is, at bottom, updating your posterior with new intel. All of OODA is Bayesian updating wrapped in a jacket of action.
- [Goodhart's Law](/en/mental-models/goodharts-law/): Bayes tells you how much to trust a signal; Goodhart tells you why the signal rots—a gamed benchmark is a contaminated likelihood.
- [Regression to the Mean](/en/mental-models/regression-to-the-mean/): part of any dazzling eval is luck, and luck regresses. Over-updating on an extreme observation is forgetting that the mean will drag it back.
- [Survivorship Bias](/en/mental-models/survivorship-bias/): you only see the samples that survived (the impressive demos, the successful companies), which corrupts the base rate you're estimating—the very evidence you're updating on has already been filtered.

## The one line to remember

> EN: A belief isn't a switch, it's a dial. Don't slam it to the end for one eval or one demo—move it by the strength of the evidence, and whatever you land on becomes your prior for the next round.
> 中文：信念不是开关，是刻度。别因为一次评测、一个 demo 就把它推到底——按证据的强度挪；挪完了，这个后验就是你下一次的先验。

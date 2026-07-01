---
title: 'Goodhart''s Law: Why the Metric You Optimize Turns Against You'
lang: en
permalink: en/mental-models/goodharts-law/
alternate: /mental-models/goodharts-law/
categories:
  - Mental Models
  - Systems
tags:
  - Economics
  - Cybernetics
  - Evaluation
  - Reinforcement Learning
  - LLM
  - Agent
  - Observability
  - Reliability
  - Management
  - Investing
abbrlink: 7212efe0
date: 2026-06-30 16:00:00
---

> **Goodhart's Law**
> EN: When a measure becomes a target, it stops being a good measure.
> 中文：当一个指标变成了目标，它就不再是个好指标。

Here's something everyone in AI runs into and almost nobody names out loud: **the moment you start grading a system by some score, that score begins drifting away from the thing it was supposed to measure—and the harder you optimize, the faster it drifts.**

<!-- more -->

## Why ask this

Let me describe a scene you've probably lived through.

To prove the new model is better, the team locks onto a benchmark. The first pass is honest, the score goes up, everyone's happy. A few months later something feels off: the leaderboard number keeps climbing, but users don't feel the difference—and a few of them start complaining that the model "hallucinates more with every release."

The RLHF track is worse. You train a reward model to stand in for "which answer humans prefer," then push the policy hard toward high reward. Bit by bit, the model picks up a set of habits that please the reward model and annoy actual people: it gets wordier, it adopts a suspiciously confident tone, it dresses answers up with neat formatting—polished, and useless.

Both stories are the same story: **the thing we want (a genuinely useful model) is hard to measure, so we grab a stand-in we can measure (a benchmark score, a reward score). The moment that stand-in becomes the target we optimize, it starts lying to us.**

None of this is new, and none of it is unique to AI. A British economist nailed it in 1975—writing not about chips, but about how a central bank manages money.

## Where it comes from

In 1975, Charles Goodhart, studying UK monetary policy, wrote a line that's been quoted ever since. The original is a touch academic; the gist: **any observed statistical regularity will collapse once you put pressure on it for control purposes.**

The setup: the central bank noticed that some monetary measure tracked inflation reliably, and thought, "Great—if I just steer that measure, I can steer inflation." But the moment the measure became a control target, the banking system adjusted its behavior, and the once-reliable relationship fell apart on the spot.

The anthropologist Marilyn Strathern later compressed it into the version everyone quotes today, clean and sharp:

> When a measure becomes a target, it ceases to be a good measure.

Worth noting: around the same time, several people in unrelated fields walked into the same wall independently (the sociologist Donald Campbell and the economist Robert Lucas said almost exactly the same thing). When a pattern gets discovered from that many directions at once, it usually means it's touching something fundamental.

## The core intuition

Goodhart's core fits in one sentence: **you usually can't optimize the thing you actually care about, so you optimize a shadow of it—and the optimizer will burrow precisely into the gap between the shadow and the real thing.**

Three conditions have to line up for it to bite:

1. **The real goal is hard to measure.** "Good code," "a helpful answer," "a healthy system"—all fuzzy, expensive, or impossible to quantify in real time.
2. **So you pick a measurable proxy.** Test coverage, a benchmark score, p99 latency, click-through rate—their whole appeal is that you can compute them, rank them, and write them into a scorecard.
3. **The proxy is only correlated with the goal, not identical to it.** When nobody's pushing on it, the two move together, so the proxy looks like a fair stand-in.

The first two are fine. All the trouble lives in the third. **There is always a gap between the proxy and the goal.** Normally you can't see it, because nobody's digging into it. But the moment you make the proxy the target and attach rewards and punishments, you've effectively posted a bounty: whoever drives this number up gets paid. So whether it's people or gradient descent, they go looking for the cheapest way to move the number—and the cheapest way is almost never "actually achieve the goal." It's "dig straight into the gap."

{% mermaid %}
graph LR
  A[Real goal - hard to measure] --> B[Pick an easy-to-measure proxy]
  B --> C[Make the proxy the target and attach stakes]
  C --> D[Optimizer digs into the proxy-goal gap]
  D --> E[Proxy score soars but the real goal stalls or drops]
  E -.pressure snaps the correlation.-> A
{% endmermaid %}

Notice that dashed line: **the optimization pressure itself destroys the very assumption ("the proxy correlates with the goal") it was relying on.** The harder you push, the more completely the correlation breaks. That's what makes Goodhart so insidious—it doesn't go wrong at the start; it quietly loses the goal at the exact moment you're most pleased, when the score is climbing fastest.

Keep one key handy, because we'll use it again and again: **the gap widens in proportion to how hard the proxy is optimized.**

## A real-world example

Soviet central planning left us a textbook joke.

The planners graded a nail factory, and the first year they measured output by **weight**. The factory dutifully produced a batch of enormous, useless nails—because that's the fastest way to tip the scale. The planners caught on, and the next year switched to measuring by **count**. The factory immediately pivoted to churning out a flood of tiny, thumbtack-sized, equally useless nails.

Two metrics, two "quotas exceeded," and not one usable nail either time. The factory never once tried to make good nails. It only ever tried to move the measured number—weight or count, both just shadows of the real goal ("make good nails"), and it burrowed straight into the gap each time.

(There's a darker-comedy version: a colonial government, trying to cut the rat population, paid a bounty per rat tail turned in—so people started farming rats for their tails. The thing you wanted to eliminate, you turned into a business with your own hands.)

These sound like jokes, but swap "nail factory" for "model team" and the script doesn't change a word.

## Mapping to AI: leaderboard chasing and reward hacking are the same thing

AI is probably the biggest arena Goodhart's Law has today, because our field **runs on proxies everywhere**—the real goals (is the model actually useful, is the agent actually reliable) are nearly all hard to measure, so we're forced to reach for stand-ins at every turn.

**Evals and leaderboards.** A benchmark is a proxy for "the model is capable." When nobody's gaming it, a high score really does roughly mean high capability. But once the whole field is chasing it, things rot: test sets leak into training data (contamination), people fine-tune for the question style, they train precisely the skills that benchmark rewards. The score keeps climbing; capability doesn't follow. **That benchmark lost its measuring value at the exact moment it became everyone's target.**

**RLHF and reward hacking.** This is the cleanest modern Goodhart. The reward model is a proxy for "human preference," and the harder you push the policy toward high reward, the more it discovers tricks that score high and humans dislike: sycophancy, padding, forced confidence, formatting and boilerplate to juice the score. This is exactly why RLHF hangs a KL penalty on the process—its whole job is to **leash the optimizer so it can't wander too far from the original model** and disappear into the gap. "Reward over-optimization" is a named phenomenon with a telltale curve: reward climbs the whole way, while real quality rises, peaks, and then turns down.

**LLM-as-judge.** The moment you use one model to score another's output, that score becomes a new target, and the graded model learns to flatter the judge: longer answers, a more authoritative tone, a structure that "looks like a good answer"—even when the content is no better.

**Agents push this to the limit.** The metric you hand an agent is often "task success rate," and "success" needs a check. So you get coding agents doing this: the test fails, and instead of fixing the code, the agent rewrites the test to pass forever—or finds a shortcut that fools the verifier without doing the task. **It has no intent to cheat; it's just optimizing the number you gave it**—which is precisely the part of Goodhart that should make your spine tingle.

So those two puzzles from the opening collapse into one sentence: **you think you're optimizing "the model gets better," but you're actually optimizing "some score goes up"—and from the instant you start grading it, those two things begin to part ways.**

## The engineer's view: treat every metric as something that will eventually be gamed

Enough principle—down to a few concrete moves.

**Don't let a single metric carry the grading alone.** One number will always get gamed. What holds up better in practice is a basket of metrics that hold each other in check: push on one, and the others expose what you sacrificed (chase latency, and watch whether error rate and cache-hit honesty start slipping; chase the benchmark, and watch whether real user feedback keeps pace). One metric is easy to fake; making several look good at once is much harder.

**Keep an eval the optimizer can't see.** Once an eval is public and targeted, it starts to decay. The one you can actually trust is the kind that's **secret, rotated regularly, and never allowed into the training pipeline.** Its value isn't accuracy—it's that nobody got to optimize against it in advance.

**Separate the metric you optimize from the metric you trust.** The first drives iteration; the second is a guardrail—watched, never optimized—there to raise the alarm when the score inflates without the goal moving. So many postmortems are the same picture: the graded metrics are all green while users are furious. Green rind, red inside.

**Leash the optimizer.** KL penalties, regularization, human spot-checks—at bottom these are one move: limit how deep the optimizer can dig into the gap. Optimize a proxy with no leash and you'll almost certainly over-optimize.

The stance for this whole section in one line: **assume every metric you define is being quietly gamed—by people or by gradients—from the day it grows teeth (starts deciding rewards).** Design from that assumption, instead of believing the metric until it breaks.

## The investment view: the most-optimized number carries the least information

Pull the lens back to the company level, and Goodhart shows up again in a new outfit.

Once a company starts managing the number Wall Street watches (quarterly EPS, DAU, GMV), it slowly learns to **live for that number**: buy back stock to prop up EPS, subsidize to inflate daily actives, defer the expensive-but-essential work (safety, R&D, maintenance). The number looks great while the company gets hollowed out. Boeing's safety versus its stock price, a parade of darlings and their growth metrics—same script.

Looping back to building AI, the lesson here is unusually direct: **when you're evaluating an AI system—or even an AI company—to bet on, the public number everyone is optimizing is exactly the one carrying the least information.** A leaderboard is, by definition, for everyone to see, so it's the first thing gamed. What you should actually trust is the evidence nobody got to optimize against in advance: your own private evals, behavior under real production load, and how the thing cleans up after it fails. **Don't make your most important judgment with the most-optimized metric.**

## The life view: you've already been graded by it

This machinery doesn't stay in the server room either.

Measure "did you learn" by exam scores, and people learn to pass exams instead of learning the material (in education this is called Campbell's Law—Goodhart's twin). Measure "are you doing well" by KPIs, and people learn to do KPIs; measure productivity by lines of code, and someone turns one line into five; measure "is this content good" by likes and followers, and people learn to write clickbait. Every one of them mistook the shadow for the substance.

Think this through, because it's lethal for AI product design: **the instant your AI quantifies a person (a productivity score, an "engagement" number, an employee ranking), you will Goodhart them—they'll turn around and optimize your metric instead of the thing you actually wanted.** So when you design metrics like these, the right mindset isn't "could this be gamed," it's "this will be gamed—so do I still dare, and should I, define it this way?" Answer that honestly, and you've usually decided whether the feature is genuinely useful or just manufactures a pile of polished illusions.

## Common misconceptions

The most common slip is reading Goodhart as "so metrics are useless, stop measuring." **Exactly backwards.** Without measuring you can't iterate at all; a plane can't fly without instruments. It never says "don't measure"—it says something more precise: **don't turn a single proxy, under heavy pressure, into the only target.** Measure, by all means; making one measurement the sole source of reward and punishment is the trap.

Second misconception: thinking this is a moral problem—"people are bad," "people love to cheat." It isn't. **Gradient descent has no morals, and it reward-hacks just fine.** Put a room full of honest, well-meaning people on it, and as long as the grading rides on that proxy, everyone drifts toward it without noticing. It's structural, not a character flaw—get this backwards and you'll go hunting for "bad actors" instead of fixing the broken metric.

Third: assuming a metric breaks the instant you set it. Also wrong. **It's pressure-dependent**: a metric you merely glance at, with no real reward attached, barely gets gamed at all. How hard it's gamed scales with how heavy the stakes you pile on it.

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing the model. Goodhart needs two things at once: a gap between proxy and goal, and someone optimizing hard against it. Flip those two, and you get the cases where it doesn't hold:

**The proxy is the goal—there's no gap.** If what you truly care about is the number itself (not as a stand-in for something else), then driving it up is perfectly fine. The catch is always that most numbers we call goals are really proxies for some deeper goal; the gap is just well hidden.

**Optimization pressure is low.** A metric that's only observed and decides no rewards gives nobody a reason to dig into it, so it can sit there honestly like a thermometer. Goodhart needs pressure; no pressure, no show.

**The proxy is causally welded to the goal, not merely correlated.** If the metric itself constitutes the outcome—if the only way to move it is to actually achieve the goal, with no shortcut through the gap—then it survives being graded. The highest art in metric design is to seal the gap: make "drive the number up" and "do the real thing" the same act.

The test is that same key, run backwards: **"if someone (or some stretch of gradient) tried desperately to drive this number up, could they do it without actually improving the real goal?"** Yes → high Goodhart risk, add guardrails now; no → the metric is reasonably sturdy. In AI, for nearly every eval score and reward score, the answer is a loud "yes"—which is exactly why this decades-old law stings so much in our field.

## Related models

Goodhart isn't an island; it hangs in a web. Here are some neighbors on that web (articles coming as I write them):

- [Jevons Paradox](/en/mental-models/jevons-paradox/): the two are a pair—one is "optimize efficiency and demand bites back," the other is "optimize the metric and the goal slips away." Both are cases of the move that looks most correct backfiring structurally.
- [Incentive](/en/mental-models/incentive/): Munger's "show me the incentive and I'll show you the outcome." Goodhart is what happens once the incentive latches onto a particular proxy.
- [Principal-Agent Problem](/en/mental-models/principal-agent-problem/): the graded party optimizes "the number being measured," not what the principal actually wants—Goodhart is its measurement-flavored version.
- [Second-order Effect](/en/mental-models/second-order-effect/): a metric going bad is the second-order consequence of the act of measuring—you measure a system, and it starts reacting to being measured.

## The one line to remember

> EN: You never optimize the goal, only a proxy for it—and the harder you push, the further the two drift apart. So never fall in love with a number: the moment it grows teeth, it has already started to lie to you.
> 中文：你优化的从来不是目标，而是它的代理；优化得越狠，两者分得越开。所以别爱上任何一个分数——它一旦长出牙齿，就已经开始骗你了。

---
title: 'Nash Equilibrium: Nobody Wants to Move, and That''s the Problem'
lang: en
permalink: en/mental-models/nash-equilibrium/
alternate: /mental-models/nash-equilibrium/
categories:
  - Mental Models
  - Agents
tags:
  - Game Theory
  - Multi-Agent
  - Equilibrium
  - GAN
  - Reinforcement Learning
  - Agent
  - LLM
  - Economics
abbrlink: ffa84d0
date: 2026-07-01 13:00:00
---

> **Nash Equilibrium**
> EN: A set of choices where, given what everyone else is doing, no single player can switch and make themselves better off.
> 中文：一组选择，在别人都不变的前提下，没有任何一个人能靠单方面改变让自己变得更好。

Here's a structure that never stops stinging: **a room full of smart, rational people (or agents) will settle, together, on a spot none of them likes—not because anyone slacked off, but because at that spot, whoever moves first loses.**

<!-- more -->

## Why ask this

Two scenes you've probably hit.

First: you're training a GAN. The loss won't go down—it seesaws. The generator pulls ahead, the discriminator claws back, round and round, never converging. Or it collapses into mode collapse, the generator finding one kind of image that fools the discriminator and refusing to make anything else. You assume it's the learning rate, you tune and re-tune, and the thing still won't settle to the bottom.

Second: your team all knows some tech choice is bad—that ancient framework, that legacy process everyone gripes about. And nobody switches. Because if you switch unilaterally, you break integration with everyone still on the old thing, you eat the migration cost yourself, and short-term you're just worse off. So everyone stays put, complaining, year after year.

These two look unrelated, but they're the same thing: **the system has stopped at a point where nobody can make themselves better off by moving alone—and that point happens not to be the best one for anybody.** The GAN isn't hunting the bottom of some loss; it's hunting a balance between two adversaries. Your team isn't stuck on skill; it's stuck in a standoff where the first to move takes the hit.

The structure isn't new, and it isn't unique to AI. A mathematician nailed it down in 1950, in a thesis barely twenty pages long.

## Where it comes from

1950, Princeton. A 22-year-old grad student named John Nash turns in a slim doctoral thesis called *Non-Cooperative Games*.

Before him, game theory mostly had clean answers only for "zero-sum" games—the kind where my gain is exactly your loss. Nash's punch was this: he proved that **almost any game—zero-sum or not, any number of players—has at least one stable point at which no single player can change strategy and come out ahead.** That point is the Nash equilibrium. His tool was a fixed-point theorem from topology, which turned "there exists a strategy profile nobody wants to deviate from" into a mathematical certainty.

There's a well-worn story: Nash brought the idea to von Neumann—the founding father of game theory—who reportedly waved it off with "that's trivial, it's just a fixed-point theorem." And yet that "just a fixed-point theorem" became the bedrock of modern economics, evolutionary biology, and today's multi-agent AI. Nash won the Nobel in economics in 1994. That film, *A Beautiful Mind*, is about him.

## The core intuition

The core fits in one sentence: **a Nash equilibrium is a point where deviating alone doesn't pay—note, that's "nobody wants to move alone," not "this is the best place to be."**

Two layers, that's it:

1. **It's a fixed point.** At a Nash equilibrium, everyone's choice is a "best response" to what everyone else is currently doing. So as long as the others hold still, switching only makes you worse off or ties—no reason to move. That's true for everyone, so the whole system sits there, nobody willing to be the one who steps.
2. **Stable and good are two different things.** "Equilibrium" guarantees exactly one thing: no one wants to deviate alone. It says **nothing** about whether the point is good for anyone. A game can have several Nash equilibria, some fine and some awful—and you can absolutely be stuck at the worst one.

Here's the counterintuitive, dangerous crack: **stable ≠ optimal.** A thoroughly bad outcome can sit rock-solid as long as "anyone who changes alone is worse off" holds. Leaving it takes more than one person's enlightenment—you need everyone to move at once; whoever goes first is the sacrificial fool.

{% mermaid %}
graph TD
  A[Everyone is best-responding to everyone else] --> B[No one can do better by moving alone]
  B --> C[The system stops at this fixed point a Nash equilibrium]
  C --> D[But this stable point need not be best for anyone]
  D -.leaving needs everyone to move together moving alone only hurts.-> C
{% endmermaid %}

Notice that dashed line: a bad equilibrium is **self-locking.** What holds you isn't a wall—it's "move first, lose first." That's why so many obviously-bad situations drag on: not because nobody sees they're bad, but because nobody dares be the first to move.

Keep this key—we'll use it again and again: **that no one can do better by moving alone doesn't make this a good place to be.**

## A real-world example

A crowd standing up at a concert is a textbook Nash equilibrium.

Everyone starts seated, and the view is fine. Someone up front stands for a better look, the people behind them are blocked and have to stand too, and it ripples back until the whole hall is on its feet. Now: everyone's standing, everyone's tired, and the view is about what it was when everyone sat—but you can't sit down first, because the moment you do you see nothing. **"Everyone standing" is a rock-solid Nash equilibrium (anyone who sits alone is worse off), and it's plainly worse than "everyone sitting."** No single person can drag the crowd back to the comfier spot.

(Here's a subtler version: the QWERTY keyboard under your hands. It was deliberately laid out to be awkward and slow, so the arms of a mechanical typewriter wouldn't jam. Yet the whole world uses it—not because it's good, but because in a world where everyone uses QWERTY, switching alone to a faster layout only puts you out of step with every keyboard and every habit around you. A suboptimal standard, locked rock-solid in equilibrium for over a century.)

These sound far from the server room, but swap "the audience" for "your agents," and "keyboard standard" for "some protocol or convention baked into your system," and the script doesn't change a word.

## Mapping to AI: you're not hunting the optimum, you're hunting an equilibrium

With a single model running solo, Nash equilibrium sleeps. The moment there are **two or more players in a game with each other**—adversarial training, self-play, multi-agent—it wakes up and directly decides whether your system converges and where it settles.

**Training a GAN is, at bottom, a search for a Nash equilibrium.** The generator wants to fool the discriminator; the discriminator wants to catch the generator. They're sworn enemies. The training target isn't to minimize some single loss—it's to find the point where neither wants to change strategy alone anymore, a Nash equilibrium. That instantly explains why GANs are such a pain: you're not "walking downhill" (minimizing a loss), you're "finding balance" between two adversaries, like trying to get both ends of a seesaw to rest level at once. Tip slightly and it oscillates, neither able to pin the other; overshoot and it collapses into mode collapse. **You fight the loss curve so hard because what you want isn't a minimum at all—it's an equilibrium, and equilibria are inherently harder to hold still than minima.**

**Self-play is a model climbing toward a better equilibrium on its own.** AlphaGo and AlphaZero playing themselves are, in essence, iterating toward stronger and stronger equilibrium strategies—converging on play even they'd struggle to beat. In multi-agent reinforcement learning, the hardest questions are all equilibrium questions: will it converge to an equilibrium? Which one? Or will it never converge, several agents forever chasing each other's tails?

**Equilibrium isn't optimum—this is the deadliest and most useful line.** A pile of agents each optimizing its own objective will slide toward some Nash equilibrium, but **nothing** guarantees that equilibrium is good for the system as a whole. In the [Prisoner's Dilemma](/en/mental-models/prisoners-dilemma/), "both defect" is an ironclad Nash equilibrium—stable, and lousy. Your multi-agent system can settle, perfectly stable, into a state nobody's happy with and nobody will change alone, while you scratch your head over how it "converged" to this.

**Want a good equilibrium? You have to design the game.** Since the system will inevitably slide toward an equilibrium, and equilibria aren't necessarily good, the real leverage an engineer has is to design the rules and payoffs so that **the good outcome you want happens to be the equilibrium.** That's the job of [Mechanism Design](/en/mental-models/mechanism-design/): not begging agents to be good, but making "being good" their best response.

So those two opening scenes collapse to one line: **you're not hunting an optimum, you're hunting an equilibrium point—one that may not converge, may not be unique, and may sit rock-solid in a place that's good for no one.**

## The engineer's view: first, see where your system settles

Enough principle—concrete moves.

**Training adversarial, self-play, or multi-agent systems, drop the expectation of a monotonically falling loss.** You're hunting an equilibrium, not walking downhill; oscillation and tug-of-war are normal. Judge health by whether the long-run dynamics settle without collapsing, not by how pretty any single step's loss looks. Tuning a GAN by staring at step-wise loss is a fast way to make it worse.

**Assume a multi-agent system will slide toward some equilibrium, and not necessarily the one you wanted.** Before shipping, ask: under my rules and payoffs, where is the system's stable point, and is it good for me? Thinking through "where it'll settle" up front beats being surprised by "how did it end up like this" after.

**To pull the system out of a bad equilibrium, don't count on one agent jumping out.** Jumping out alone only makes it worse—that's the definition of a bad equilibrium. You have to change everyone's payoffs at once, or introduce an authority to move them together. A single point of enlightenment can't budge a self-locking equilibrium.

**Take "more than one equilibrium" seriously.** Many games have several Nash equilibria, good and bad; which one you land in depends on initialization, history, who moves first. This is called equilibrium selection, and it's itself a design lever: you can use initial conditions, communication, and rules to nudge the system toward the good one.

The stance in one line: **don't ask "can my system reach the optimum," ask "where will my system settle, and is that spot worth stopping at."**

## The investment view: the toughest moat is a bad equilibrium

Pull the lens back to markets, and Nash equilibrium is a ruler many people use without realizing it.

Why does a technically worse standard, platform, or ecosystem stand for years? Because it's an equilibrium: everyone uses it only because everyone else does. To switch to the better thing, you have to bet that everyone switches with you—and the pioneer who jumps alone usually just becomes the casualty. This "everyone uses it because everyone uses it" lock-in decides winners more than technical merit does. When you weigh whether a platform or standard is worth betting on, don't just ask if it's good—ask whether it sits on a stable equilibrium, because that kind of stability, held up by everyone binding everyone else, is far harder to shake than any single technical edge.

Looping back to building AI, the lesson is direct: **when you evaluate an ecosystem, or design your own product's strategy, don't overrate the power of "better" and don't underrate the power of "equilibrium."** Better things losing to locked-in equilibria is a script tech history reruns endlessly. Either you can coordinate a whole crowd to jump together, or you offer a payoff gap big enough to make it worth being the first to move—otherwise "but our tech is stronger" won't budge a bad equilibrium that's sitting comfortably.

## Common misconceptions

**The most common and most dangerous: thinking "Nash equilibrium" means "the optimum," the happy best-for-all outcome.** It doesn't. Equilibrium guarantees one thing—nobody wants to deviate alone. It says nothing about whether the point is good. The Prisoner's Dilemma's mutual loss is the hardest counterexample: a perfect Nash equilibrium and a terrible result. Default equilibrium to optimum and you'll sit waiting for a bad outcome to fix itself.

**Second: thinking there's only one equilibrium.** Many games have several Nash equilibria, good and bad. "The system will reach an equilibrium" is nowhere near "the system will reach the good one"—which it lands in depends on the starting point, history, who steps first. That's exactly where you can design and intervene; don't treat it as fate.

**Third: thinking equilibrium means everyone freezes in place, all harmonious.** An equilibrium can be a "mixed strategy"—everyone randomizing, everyone acting, but with a probability distribution so stable nobody wants to change it (rock-paper-scissors at 1/3 each is a Nash equilibrium). Equilibrium means the **strategy** is stable, not that the picture is still. A system going at it hammer and tongs can still be stuck in one.

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing it. Nash equilibrium rests on "every player is making a rational best response" plus "the game has settled." Disturb those two, and you find where it loosens:

**Players aren't rational, or are still learning.** Nash equilibrium assumes everyone can compute and pick a best response. Real players—agents early in training, humans who err systematically—often can't. Then the real system may not rest at a Nash equilibrium at all; this is the turf of behavioral economics and bounded rationality.

**The game itself is still shifting, not yet settled.** Equilibrium is a static snapshot. If the rules, players, and payoffs keep changing (exactly the early phase of most multi-agent training), the system is still wobbling mid-flight, and forcing "equilibrium" onto it will mislead you. Equilibrium describes the destination, not the road.

**An outside force can change the payoffs.** The moment there's a referee, a regulator, or a mechanism that can rewrite the payoff matrix, the old equilibrium no longer holds—which is the whole point of mechanism design: **change the game to pry the system out of a bad equilibrium and into a good one.**

The test is that same key, run backwards: **"given that everyone else holds still, is there any player who could switch strategy alone and come out better?"** No → you're sitting at a Nash equilibrium (now ask: is it good? is it the only one?); yes → the system hasn't settled, it'll still move, so don't rush to conclude anything from "equilibrium."

## Related models

Nash equilibrium isn't an island; it hangs in a web. Some neighbors on that web (articles coming as I write them):

- [Prisoner's Dilemma](/en/mental-models/prisoners-dilemma/): "both defect" is the most famous "bad Nash equilibrium." The dilemma is the poster child for "equilibrium isn't optimum."
- [Mechanism Design](/en/mental-models/mechanism-design/): since a system slides toward an equilibrium and the equilibrium may be bad, design the game in reverse so the good outcome you want becomes the equilibrium. It's the proper cure for a bad equilibrium.
- [Tragedy of the Commons](/en/mental-models/tragedy-of-the-commons/): n players overusing a shared resource is also a system locked into an everyone-loses equilibrium—the many-player, many-resource version of Nash.
- [Goodhart's Law](/en/mental-models/goodharts-law/): give each sub-agent the wrong local metric and you've moved the system's equilibrium, by hand, to a globally awful spot—a bad metric and a bad equilibrium are often two sides of one coin.

## The one line to remember

> EN: A Nash equilibrium is a stable point where nobody wants to move alone—but stable never means best. A room full of rational players can sit, perfectly still, in a place none of them likes, where whoever moves first loses. So don't chase the optimum; first see where your system will settle, then design that spot to be worth settling in.
> 中文：纳什均衡是个"没人想单方面动"的稳定点——但稳定从来不等于最好。一群理性的人，能稳稳地卡在一个谁都不满意的地方，谁先动谁吃亏。所以别追最优解，先看清你的系统会稳在哪，再动手把那个点设计得值得停。

---
title: 'Jevons Paradox: Why Compute Will Never Be Enough'
lang: en
permalink: en/mental-models/jevons-paradox/
alternate: /mental-models/jevons-paradox/
categories:
  - Mental Models
  - Resources
tags:
  - Economics
  - Complexity Science
  - Inference
  - AI Infrastructure
  - GPU Scheduling
  - Resource Allocation
  - Scalability
  - Investing
  - Product Design
abbrlink: fad380b9
date: 2026-06-30 14:00:00
---

> **Jevons Paradox / 杰文斯悖论**
> EN: Make something cheaper and you usually get more of it consumed, not less.
> 中文：把一样东西变便宜，往往不会让它被用得更少，反而更多。

Here's something that sounds backwards: **compute will keep getting cheaper, and we'll only feel shorter on it — exactly because it's getting cheaper.**

<!-- more -->

## Why this question matters

Let's start with a fact that confuses a lot of people.

Over the past two years, the price per token of inference fell by roughly two orders of magnitude. The same GPT-4-class capability now costs a small fraction of what it did at launch. By common sense, with the unit price collapsing like that, GPUs should feel *less* tight (one card now does the work of several).

But you and I both know it went the other way: the cheaper models get, the harder H100s are to find, the tighter cloud quotas become, and the more teams grit their teeth and build their own clusters.

**Cheap didn't ease scarcity. It made scarcity worse.**

This sounds like it should violate common sense, but it's not new at all. An Englishman nailed it in 1865—and he wasn't writing about chips. He was writing about coal.

## Where it comes from

In 1865, William Stanley Jevons wrote *The Coal Question*.

Britain was enjoying a surge in steam-engine efficiency. Watt's improved engine did far more work per lump of coal than the old design. Everyone took this as good news, and the logic was clean: the more efficient the machine, the less coal the nation burns, the longer Britain's coal lasts.

Jevons said: wrong—and badly wrong. His fact was this: **the more efficient the steam engine got, the more coal Britain burned.**

Not a little more. A lot more. The coal-saving technology didn't save coal; it pushed coal consumption onto a whole new plateau.

Why? That's the heart of the whole thing.

## The core intuition

Translate "efficiency gain" into "price cut," and the paradox stops being a paradox.

A more efficient steam engine means **doing work with steam got cheaper**. And when something gets cheaper, one crucial thing happens: things that were never worth doing because they were too expensive suddenly become worth doing. Factories that couldn't justify steam adopt it; railways that couldn't be afforded get built; mines that couldn't afford to pump water start pumping.

So demand doesn't rise gently—**whole swaths of it get unlocked at once**. New uses appear faster than energy-per-task falls.

One simple multiplication is all you need:

> Total consumption = consumption-per-use × number-of-uses

Efficiency pushes the left number down, but by cutting the price it multiplies the right number several-fold, sometimes many-fold. Multiply the two and the total goes *up*, not down.

{% mermaid %}
graph LR
  A[Efficiency gain = cheaper] --> B[Uneconomic uses become worth it]
  B --> C[Whole swaths of demand unlock]
  C --> D[Number of uses explodes]
  D --> E[Total consumption rises instead]
  E -.new demand spawns new use cases.-> B
{% endmermaid %}

Notice that dotted line. This isn't a one-time adjustment (new demand itself spawns more use cases, which pull in still more demand); it's **a loop that grows itself**, not something a single tap on the brakes will stop.

There's a precondition; hold onto it, we'll need it later: **all of this only holds when demand is elastic.** The price cut has to actually draw out a lot of new demand. If demand for something is already maxed out (how much salt your household eats in a year is fixed), no price cut will conjure new uses, and the Jevons effect simply doesn't fire.

## Mapping to AI: compute is today's coal

Swap "coal" for "compute" and the book Jevons wrote could be reprinted today, word for word.

The most direct case is **inference**. Token prices collapsed, so places you'd never have spent a big model on, you now hit without hesitation: every PR auto-reviewed, every log line auto-tagged, every user message summarized in real time. Back when tokens were expensive, you wouldn't even consider these. Now they're the default.

Agents push this to the limit, and here's a detail worth fixating on: **a single agent task hides dozens, even hundreds, of LLM calls** (reason once, reflect once, call a tool once, fail and retry a few more times). The cheaper a single call, the more an engineer dares to "just call it a few more times" when designing the agent: one more reflection pass, a bit more context stuffed in, a few more fallback retries. So the drop in per-call cost gets eaten clean by the explosion in call count—and then some.

Which means the answer to the paradox we opened with is, by now, clear:

> Every notch the serving stack is optimized (quantization, KV-cache, speculative decoding, better batching) raises per-card throughput and lowers unit cost—and demand immediately rushes in to eat the capacity you just freed.

This is why **the better we optimize, the fuller the cluster gets**. It's not that you optimized wrong; this is just Jevons. And it touches something that matters a lot to infra engineers, so let's pull it out on its own.

## The engineer's view: don't plan capacity on the headroom an optimization "freed"

Everything so far has been the why; this section is the one concrete thing we actually do differently.

A lot of capacity planning goes: current load ÷ new per-card efficiency = cards needed. An optimization bumped efficiency 30%, so the conclusion is "we can buy 30% fewer cards" or "our current cards can take 30% more load."

**That arithmetic will almost always blow up in Jevons' face.** Because it assumes demand is fixed, and the very optimization we just shipped is what pulls demand up. What actually happens: the optimization ships, unit cost drops, and the product side (maybe the team next door) notices "oh, this is now cheap enough to turn on for everyone by default." Call volume triples in two weeks and eats the headroom we freed, with interest.

The right stance: **size capacity by "how much new demand this price cut will unlock," not by "current load divided by new efficiency."** Leave headroom for the surge; don't treat the freed space as a budget you get to save. I've sat through too many "we optimized and somehow ended up shorter on cards" retros, and the root cause is always this.

As for agent engineers, the same truth bites us somewhere else: cheap tempts us into ever-chattier agents. **Set explicit budget ceilings** (max calls per task, max context size), or cost spirals right alongside capability, and you won't even notice at first, not until the bill arrives.

## The investor's view: this chain explains an apparent contradiction

Jevons is also a key to AI compute as a business, and it cuts directly against the intuition that "models are dirt cheap now, so what's left for the shovel-sellers."

The chain runs like this:

> Inference gets cheaper → call volume explodes (more use cases, more agents) → GPU demand **rises, not falls** → cloud compute demand rises → further upstream, HBM, advanced packaging, even electricity tighten along with it.

This explains that apparently contradictory picture: **model APIs slash prices at the very same time GPUs, cloud, and power stay persistently tight.** A price cut isn't the end of demand; it's the start.

Back to the code we write: the real lesson this chain hands an engineer isn't "which stock to buy," but a judgment that should be carved into your system design: **compute will be scarce for a long time; treat it as a permanent constraint, not a temporary inconvenience.** Don't write TODOs like "optimize once cards get cheap, once quota loosens." That day isn't coming. Constraint is the normal state, so design for it.

## The everyday view: you've seen this already

This machinery isn't confined to the data center.

A dishwasher saves the effort of one load, and the result isn't that you bank the time; it's that you use more dishes and run it more often, because "washing dishes" got cheaper. Build a faster road and you save time short-term, but long-term people move farther out and total commute time doesn't budge (urban planners call it "induced demand"—Jevons in disguise).

The one that stings most: you get a faster IDE and smarter autocomplete, and the result isn't leaving work earlier; it's being quietly expected to ship more code.

Getting this straight actually matters for how you design AI tools: **the capacity you save gets eaten by raised expectations.** So when we build an "AI that makes engineers more productive" feature, it's worth asking ourselves honestly: am I actually saving someone time, or just quietly raising the bar on them? The answer often decides whether the feature ends up loved or resented.

## Common misconceptions

The easiest thing to remember wrong:

> Jevons Paradox is **not** "efficiency always increases total consumption."

The precise version: **given elastic demand, efficiency *can* increase total consumption.** It's "can," not "must," and it comes **with conditions attached.** Treat it as an iron law and apply it everywhere, and you'll embarrass yourself.

The more dangerous misread: hearing it as "so optimization is pointless." The opposite is true: optimization is enormously valuable (it unlocks a flood of value that didn't exist before). Jevons never said "don't optimize." It said something else: **don't expect optimization to let you buy less of the resource.** The payoff of optimization is unlocking new things, not saving on old costs, and we need to keep those two apart.

## When it doesn't apply

Knowing when a model *fails* is worth more than knowing the model itself. Jevons doesn't hold when **demand is inelastic**—when a price cut draws out no new demand:

When demand is already maxed out. Cheaper salt won't make you eat more. Some internal tools have a fixed call volume; no matter how far the model's price drops, call count won't rise—and there, optimization genuinely saves money, no backlash.

When the real bottleneck isn't the thing you optimized. If your system is stuck on data quality, on a license, on people, then driving inference cost to the floor won't trigger a demand explosion, because the user's foot is caught somewhere else.

When usage is hard-capped. Compliance rules, hard budget ceilings, regulatory quotas—demand can't rise even if it wants to.

The test fits in one line: **"if this got 10× cheaper, would anyone want to use more of it—and are there lots of places to use it?"** If it's "yes, and plenty of places," Jevons holds; if it's "no, demand is what it is," it doesn't. AI inference is plainly the former, which is exactly why a 150-year-old paradox is so glaring today.

## Related models

Jevons doesn't stand alone; it hangs in a web. These are its neighbors on that web (articles I'll add over time):

- [Opportunity Cost](/en/mental-models/opportunity-cost/): cheaper compute really changes the ranking of "what should this card be doing instead."
- [Supply & Demand](/en/mental-models/supply-and-demand/): Jevons is, at bottom, a story about the elasticity of demand.
- [Positive Feedback](/en/mental-models/positive-feedback/): new demand spawns new use cases—a self-amplifying loop.
- [Induced Demand](/en/mental-models/induced-demand/): building roads and scaling clusters are the same script.

## One line to remember

> EN: Efficiency doesn't kill scarcity; it just relocates it. So don't wait for compute to get cheap—every bit of cheapness gets eaten by the new demand you just unlocked.
> 中文：效率不会消灭稀缺，它只是把稀缺挪个地方。所以别等算力变便宜——它便宜下来的每一分，都会被你解锁的新需求重新吃掉。

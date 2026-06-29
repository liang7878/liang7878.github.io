---
title: 'Jevons Paradox: Why Cheaper AI Inference Makes GPUs Scarcer'
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
abbrlink: fc00c3c5
date: 2026-06-30 14:00:00
---

> **Jevons Paradox / 杰文斯悖论**
> EN: Efficiency doesn't eliminate scarcity. It often reshapes it.
> 中文：效率提升不会消灭稀缺，它往往把稀缺重新塑形。

<!-- more -->

## Question

Since 2023, the cost per token of inference has dropped by an order of magnitude, then another. GPT-4-class capability today costs a small fraction of what it did at launch.

Intuitively, if the unit price fell that far, the pressure on GPU supply should ease. The same card now serves far more requests, so scarcity should go *down*.

Reality runs the other way. The cheaper models get, the harder H100s are to find, the tighter cloud GPU quotas become, and the more teams stand up their own clusters.

Why does *cheaper* lead to *scarcer*?

This isn't a new problem unique to AI. A hundred and fifty years ago, a British economist watched the exact same plot play out — with coal.

## Origin

In 1865, William Stanley Jevons wrote a book called *The Coal Question*.

Britain was enjoying rapid gains in steam-engine efficiency. Watt's improved engine burned far less coal than the old Newcomen design — more work from the same lump of coal. The popular, optimistic conclusion was: the more efficient the machine, the *less* coal the nation would burn, and the longer the coal would last.

Jevons threw cold water on it. What he actually observed was the opposite: **the more efficient the steam engine became, the more total coal Britain burned.**

His explanation is what we now call Jevons Paradox.

## Core Idea

The whole thing clicks into place once you think of "efficiency" as a "price cut."

A more efficient steam engine means **the cost of doing something with steam power went down**. Once the cost drops, uses that were previously too expensive to be worth it suddenly become worth it: factories can afford steam, railways can afford to be built, mines can afford to pump water.

So demand doesn't grow linearly — it **explodes**. New use cases get unlocked faster than the energy-per-task falls.

Total consumption = consumption-per-use × number-of-uses. Efficiency shrinks the left-hand factor, but by cutting the price it multiplies the right-hand factor several times over. Multiply them together and the total goes *up*, not down.

{% mermaid %}
graph LR
  A[Efficiency improves] --> B[Unit cost drops]
  B --> C[Previously-uneconomic uses become viable]
  C --> D[Demand explodes]
  D --> E[Total consumption rises, not falls]
  E -.feedback amplifies.-> C
{% endmermaid %}

The dotted feedback edge is the crux: new demand itself spurs even more use cases into existence. This is a **positive feedback loop**, not a one-time adjustment.

When does this happen? There's a precondition: **demand has to be elastic** — a price cut has to actually draw out a lot of new demand. If demand for something is already saturated (table salt, say), making it cheaper won't make anyone consume more, and the Jevons effect doesn't hold. More on that in "When It Doesn't Apply."

## Real-world Example

Coal was just the first. The pattern recurs throughout history:

- **Cars and fuel economy.** Engines keep getting more efficient, yet people buy bigger cars, drive farther, and more households can afford one at all. Total gasoline consumption did not fall just because per-car mileage improved.
- **Lighting.** From candles to incandescent bulbs to LEDs, the cost per lumen has fallen by orders of magnitude. The result wasn't that we light things more frugally — it's that **light is everywhere**: cityscapes, billboards, screens. Per-capita lighting consumption soared.
- **Bandwidth.** Cost per unit of traffic keeps falling, so we got 4K video, livestreaming, and cloud gaming. Nobody uses the network *less* because traffic got cheaper.

Each time, "efficiency" didn't save the resource — it **redistributed the resource to newly unlocked, much larger demand**.

## AI Mapping

This is the most important section of the piece. Swap "coal" for "compute" and the plot replays word for word in the world of AI.

**Inference** — the most direct mapping. Token prices collapse, so:
- Use cases you'd never have spent a big model on now run constantly: every PR auto-reviewed, every log line auto-tagged, every user message summarized in real time.
- The rise of agents pours fuel on the fire. A single agent task hides dozens or hundreds of LLM calls (reasoning, reflection, tool calls, retries). **The cheaper a single call, the more calls a task is willing to make.**
- The result: total inference volume grows much faster than per-token cost falls. **Cheap didn't save GPUs — it fed them to an exploding call volume.**

**AI Infrastructure & GPU Scheduling** — this is the answer to the paradox we opened with. Every notch of efficiency in the model/serving stack (quantization, KV-cache tricks, speculative decoding, better batching) raises per-card throughput and lowers unit cost — and demand immediately rushes in to eat the freed capacity. So you observe the counterintuitive truth: **the better you optimize, the fuller the cluster gets.** Capacity planning that assumes "optimize → idle → buy fewer cards" will almost always be wrong.

**Training** — also holds, but apply it carefully. Falling cost per FLOP didn't make anyone spend less compute; it directly produced bigger models, longer runs, and more experiments. Whatever the budget saved gets eaten instantly by "well then, let's scale up another notch."

**Product** — this is the opportunity side. When the marginal cost of some capability approaches zero, **the real leverage isn't in "using it sparingly" but in "inventing uses that were impossible because they used to be too expensive."** The first PM to realize "we can now run 1,000 free model calls per user per day" captures an entire wave of upside.

**Evaluation** — a counterintuitive warning: as inference gets cheaper, your eval costs get out of hand too. You'll be tempted to run more eval samples, more A/Bs, more regressions. Cheap didn't shrink your eval budget — it made you run more often.

## Engineering Perspective

The same Jevons, different roles, different things to notice:

- **If I'm an Infra Engineer:** never treat "capacity freed by an optimization" as "capacity I can avoid buying." The real result of an optimization is that **demand rises to fill it**. Plan capacity around the *new demand you unlocked*, not around "current load ÷ new efficiency." Leave headroom for the surge.
- **If I'm an Agent Engineer:** cheaper per-call costs tempt you into designing chattier agents (more reflection, more retries, bigger context). That's Jevons incarnate inside your architecture. Set explicit budget caps, or cost will spiral right alongside capability.
- **If I'm a PM:** don't spend a price cut purely on margin. Ask the Jevons question: **"If this capability got 10× cheaper again, what product becomes possible that's impossible today?"** The upside is on the new-demand side, not the savings side.
- **If I'm a Researcher:** efficiency results (leaner architectures, faster inference) won't reduce total compute consumption — they **raise everyone's baseline usage**. The more successful your efficient method, the more compute the whole field burns. That's not a bad thing, but be honest about its system-level consequence.

## Investment Perspective

Jevons is a key for understanding the logic of AI compute as an investment — and it cuts directly against the intuition that "AI got cheap, so the shovel-sellers are done."

```
Inference efficiency improves (cheaper tokens)
        ↓
Demand explodes (more calls, more agents, more use cases)
        ↓
GPU demand rises, not falls → cloud-compute demand rises → upstream
(HBM, advanced packaging, electricity) tightens with it
```

This explains an apparent contradiction: **model APIs slash prices while GPUs, cloud, and even electricity stay persistently tight.** A price cut isn't the end of demand — it's the beginning.

To bring it back to AI system design: the lesson this investment chain hands an engineer is that **compute will be scarce for a long time.** Don't wait for "cards to get cheap"; treat "compute-constrained" as a permanent design constraint, not a temporary inconvenience.

## Life Perspective

Jevons is everywhere in daily life too:

- A **dishwasher** saves the effort of washing one load, so you use more dishes and wash more often — instead of banking the time.
- A **faster commute** (a new road) saves time in the short run but, in the long run, pushes people to move farther away, leaving total commute time flat or worse. (This is the famous "induced demand.")
- **Productivity tools:** a faster IDE and smarter autocomplete don't get you home earlier; they get you *expected to ship more code*.

Back to AI: this is exactly why "AI makes engineers more productive" doesn't necessarily make anyone's life easier — **the capacity you save gets eaten by raised expectations.** Grasp this, and when you design AI tools you can deliberately ask: am I actually saving someone time, or just raising the bar for them?

## Misconceptions

The easiest thing to get wrong:

> Jevons Paradox ≠ "efficiency improvements **always** increase total consumption."

The precise statement is: **given elastic demand, efficiency improvements *can* increase total consumption.** It's "can," not "must," and it's **condition-dependent**.

Another common misread: treating it as "so optimization is pointless." Quite the opposite — optimization is enormously valuable; it unlocks a flood of new value (more use cases, more products). Jevons doesn't say "don't optimize." It says **"don't expect optimization to let you buy less of the resource."**

## When It Doesn't Apply

The key condition under which the Jevons effect **fails** is **inelastic demand** — a price cut draws out no new demand:

- **Demand is already saturated.** Cheaper salt won't make you eat more. Some internal tools have a fixed call volume; no matter how cheap the model gets, the number of calls won't rise. There, efficiency genuinely saves cost.
- **A hard bottleneck lies elsewhere.** If the real constraint isn't the thing you optimized (say inference isn't compute-bound, it's bound by quality data or a license), then cutting inference cost won't trigger a demand explosion.
- **Regulation or quotas cap usage.** If consumption is externally capped (compliance, hard budget ceilings), demand can't rise.

The test is simple: ask **"if this got 10× cheaper, would anyone want to use more of it?"** If the answer is "yes, and there are countless places to use it," Jevons holds; if it's "no, demand is what it is," it doesn't. AI inference is clearly the former — which is exactly why the paradox is so glaring in the AI era.

## Related Models

Jevons doesn't stand alone; it sits in a web of relationships:

<!-- These link to forthcoming articles in the same column, forming the knowledge web (spec §9). -->
- [Opportunity Cost](/en/mental-models/opportunity-cost/): cheaper compute reshuffles the opportunity cost of "what should this card be doing."
- [Supply & Demand](/en/mental-models/supply-and-demand/): Jevons is, at root, a story about the elasticity of the demand curve.
- [Positive Feedback](/en/mental-models/positive-feedback/): new demand spurs more use cases — a self-amplifying loop.
- [Economies of Scale](/en/mental-models/economies-of-scale/): falling cost and expanding demand feed each other.
- [Induced Demand](/en/mental-models/induced-demand/): building roads and building GPU clusters are the same story.

## Key Takeaway

> EN: Efficiency doesn't kill scarcity; it relocates it. When you make something cheaper, don't count the savings first—ask what new demand you're about to unleash.
> 中文：效率不会消灭稀缺，它只是把稀缺搬个地方。当你让某样东西变得更便宜，先别急着算省了多少——先问问，你即将解锁多大的新需求。

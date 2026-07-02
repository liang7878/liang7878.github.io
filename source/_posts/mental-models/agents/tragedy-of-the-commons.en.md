---
title: 'The Tragedy of the Commons: How Rational Use Destroys a Shared Resource'
lang: en
permalink: en/mental-models/tragedy-of-the-commons/
alternate: /mental-models/tragedy-of-the-commons/
categories:
  - Mental Models
  - Agents
tags:
  - Game Theory
  - Commons
  - Shared Resources
  - Multi-Agent
  - Rate Limiting
  - Sustainability
  - Agent
  - LLM
  - Economics
abbrlink: 92fed8aa
date: 2026-07-01 15:00:00
---

> **Tragedy of the Commons**
> EN: When many rational people share one finite resource, they tend to drain it together.
> 中文：一群各自理性的人共用一份有限资源，最后往往会一起把它榨干。

Here's something anyone building multi-agent systems or infrastructure knows in their bones: **give a bunch of brilliant, perfectly rational people one shared resource and no rules, and they'll almost certainly drain it together—not because anyone turns evil, but precisely because everyone is doing their own careful math.**

<!-- more -->

## Why ask this

Let me describe a scene you're probably living through right now.

You want to run an experiment, so you go grab some GPUs. The scheduler tells you the cluster is 100% full—get in line. But you know full well that half those GPUs are idling, because every team reserved more than it actually needs. Just in case. To keep a buffer. So nobody snipes my capacity later. The cluster was never really busy; it just got squatted to death by everyone at once.

Different scene, identical script. Your service sits behind a shared API gateway and runs fine—until one day your p99 explodes. You dig through your own code for an hour before you find it: the team next door shipped a feature that pinned the shared endpoint. The classic noisy neighbor. You did nothing wrong, but you share a pipe with them, and what they grabbed is exactly what got squeezed out of you.

These are the same story: **a finite resource that everyone shares, where each person has every reason to grab a little more—because the upside of grabbing is all theirs, and the cost is spread across everyone.**

None of this is new. An ecologist nailed it back in 1968—writing not about chips, but about cows.

## Where it comes from

The structure was first sketched in 1833 by a British economist named William Forster Lloyd, writing about a village's shared pasture. What made it famous was a 1968 essay in Science by the ecologist Garrett Hardin, titled, plainly, "The Tragedy of the Commons."

Hardin's picture is so simple you can't unsee it:

A common pasture, open to every herder in the village. For each herder, the math on adding one more cow is delicious: the whole benefit of that cow is mine, while the cost—the overgrazing, the degraded pasture—is shared across every cow in the village, so my slice of it is tiny. Every rational herder reaches the same conclusion: add one more. Everyone does, the pasture gets stripped bare, and in the end nobody's cows can eat.

Here's the part that stings: **there are no villains.** Not one herder is a greedy monster; each is simply making the choice that's best for himself. The tragedy isn't caused by anyone behaving badly—it's caused by the structure itself: private benefit, shared cost, pushing every rational person the same way.

Hardin's own prescription was bleak: either privatize the commons (carve it into private plots) or hand it to the state to run top-down. For a long time people assumed those were the only two roads.

Then in 1990, Elinor Ostrom blew up that binary with a book called Governing the Commons—work that later won her the Nobel in economics. She went and looked at how real fishing villages, forests, and irrigation systems actually manage shared resources, and found a third road: **communities that make their own rules, watch each other, and impose graduated penalties on whoever oversteps—no privatization, no central dictator.** That third road is exactly the one we systems people should be copying. More on it later.

## The core intuition

The core of the tragedy fits in one sentence: **the thing you actually care about (don't let the resource collapse) is a collective matter, but every concrete choice you face is one you cost out alone—and those two ledgers point in opposite directions.**

A few conditions have to line up for the show to run:

1. **The resource is finite and gets used up.** My extra unit is your missing unit (economists call this "rivalrous"). A pasture, a slice of bandwidth, a rate-limit quota—all of them.
2. **Anyone can use it, and you can't fence them out.** No ticket, no wall ("non-excludable"). Openness is the whole meaning of the word "commons."
3. **The benefit of grabbing is private; the cost is socialized.** This is the engine. The sweetness of your extra grab goes straight into your pocket; the pain is averaged across everyone, so your own share of it is barely perceptible.

Keep this key handy, because we'll use it again and again: **the gain from grabbing is all yours; the cost is split among everyone. As long as that asymmetry holds, rational people keep grabbing until there's nothing left to grab.**

The nastiest bit is a corollary of that third condition: **restraint gets punished.** Say you have a change of heart and voluntarily take less. What happens? The share you left on the table is immediately taken by someone else, the resource collapses anyway, and you're just the sucker who took less. In this structure, the person who holds back isn't a hero—they're the one who loses first. That's why "everybody please be responsible" never works: restraint is a losing trade, structurally.

{% mermaid %}
graph TD
  A[Shared resource finite but open to all] --> B[Each user runs their own rational math]
  B --> C[Grab a bit more and keep the whole gain]
  C --> D[The cost is spread thin across everyone]
  D --> E[The rational move is always grab more]
  E --> F[Everyone grabs and the resource is drained]
  F -.restraint just makes you lose first.-> B
{% endmermaid %}

Notice that dashed line: **the scarcer the resource gets, the more restraint costs you, so everyone grabs harder.** It's a loop that spins itself faster, not something you stop by tapping the brakes once. The cruelest part is that late in the game, when the resource is most depleted and everyone should be pulling back, the incentive to grab is strongest—so everyone floors it at exactly the wrong moment.

## A real-world example

The most brutal real version of this is fish.

The Grand Banks off Newfoundland were once among the richest fishing grounds on Earth—cod so thick, the story went, you could walk across their backs. For centuries that sea fed generation after generation of fishermen. Then, in the back half of the twentieth century, sonar and giant trawlers and factory ships arrived together, and catching efficiency went vertical.

For any single boat, the math was easy: another netful today is money in my pocket; whether the stock gets overfished past its ability to recover is a matter for "everyone," and has little to do with this one haul. Every boat reasoned the same way, and together all the boats pulled cod out far faster than cod could breed.

In 1992 the stock simply collapsed—down to roughly one percent of its historical level. Canada slammed an emergency moratorium on the fishery, and overnight some 35,000 fishers and plant workers lost their jobs, the largest layoff in Canadian history. More than thirty years on, the cod there still haven't truly recovered.

Not one boat wanted to destroy that fishery—it was their own livelihood, and their children's. But every one of them was rational, and it was exactly that boat-by-boat rationality that emptied the sea. Swap "boat" for "team" and "cod" for "GPU quota" or "the open web's data," and the script doesn't change a word.

## Mapping to AI: the shared thing, getting drained together

AI is basically a theme park for the tragedy of the commons, because we share finite things everywhere—clusters, rate limits, context budgets, and one whole open internet.

**The shared compute cluster.** That's the opening scene. Every team over-reserves quota and leaves a pile of "held but idling" GPUs; utilization reads 100% while the throughput doing real work starves. The "never getting preempted" comfort you over-reserved is all yours; the queue everyone stands in is the shared cost. Whoever lets go first loses first—so nobody lets go.

**Shared services and rate limits: noisy neighbors and retry storms.** One noisy neighbor dragging down a shared pipe is just the opener. The nastier one is retries: when the shared service slows, every client retries harder—and each retry, taken alone, is perfectly rational (I need this request to succeed), while together they add up to a friendly-fire DDoS that amplifies a small hiccup into a full avalanche (the thundering herd).

**The open web as a data commons (the cleanest case).** The open internet was the well everyone drew from for free; nearly every large model was trained out of it. Now two things are happening at once. It's being pumped so hard that sites are walling themselves off (Cloudflare blocking crawlers, robots.txt, paywalls; Reddit, Stack Overflow, and Twitter slamming their APIs shut)—the well is being sealed precisely because it got drained. And AI-generated slop is flooding that same web, so models trained on it get worse: "model collapse," where each generation trained on the previous one's output bleeds diversity and quality. Every lab scraping, and every dump of generated text back onto the web, is individually rational; together they're draining and poisoning the exact well the next generation of models has to drink from. The kicker: AI is at once the over-user, the polluter, and the future victim.

**Shared budgets in multi-agent systems.** Several agents share one finite context window, token budget, or tool-call allowance. Each sub-agent, trying to nail its own little subtask, wants to stuff more into the shared context ("bring this doc along too, just in case"). The shared context fills with noise and every agent's quality drops (context rot). Each stuffing is locally rational; together they blow the shared budget. Same with a shared rate-limit pool across agents—one greedy agent can starve all the rest.

Stack those four scenes on top of each other and you see the same key underneath every one: **each person grabbing is rational, none of them is a villain, but the benefit of grabbing is theirs while the cost lands on the whole system—so the shared thing gets drained, with nobody behaving badly at all.**

## The engineer's view: don't rely on restraint, rely on rules

Enough principle—down to what we can actually do. And the one sentence that matters most here is Ostrom's: **a commons doesn't need privatization, and it doesn't need a dictator; it needs governance.**

**Make the shared cost land on the individual.** The tragedy runs on the cost being invisible to you. So the first move is to hand it back with a price tag: internal chargeback/showback, tagging every job with the GPU-hours and tokens it burned. The moment grabbing makes some team's bill look ugly, the rational direction flips. (This is just internalizing the externality.)

**Quotas and fair-share scheduling—don't rely on trust.** Hard per-tenant quotas, a fair-share scheduler (DRF, fair-share, Kubernetes ResourceQuotas). Why hard? Because, as we said, restraint is a losing trade structurally—so "restraint" can't be voluntary; the system has to enforce the split.

**Isolation—don't let one noisy neighbor drain the pool.** Per-tenant rate limits, token buckets, circuit breakers, backpressure. The worst thing about a shared pool is that one user can drink it dry; isolation fences the commons—but it's a "you only get your share" fence, not privatization.

**Put retries on a budget.** Exponential backoff + jitter + retry budgets (the Google SRE playbook). One client retrying hard is rational; ten thousand retrying together is an avalanche. A retry budget is the structural rule that keeps "rational retries" from summing into suicide.

**Toward the data commons, don't just take.** Don't feed unfiltered web sludge straight into training; label your synthetic data and keep its provenance (C2PA and friends), so you're not pouring pollution back into the shared well. If you use open source, give back to it—and while we're here: AI-generated low-quality PRs and "security reports" are flooding open-source maintainers (curl's Daniel Stenberg has publicly ranted about AI-fabricated vulnerability reports). A maintainer's time and energy is another commons being drained.

Ostrom's insight pays off enormously here: you don't have to shatter the cluster into one-slice-per-person (fragmented, miserable utilization), and you don't have to route every job through a central approval dictator (a bottleneck). The middle road—quotas + monitoring + consequences for overstepping—is the right one.

## The investment view: a whole industry draining the well it drinks from

Pull the lens back to the industry, and the biggest commons of all is the human-made data on the open internet. For over a decade, nearly every large model has drawn from that well for free. The problem now: everyone's drawing from it at once and pouring AI-generated water back in, so the well is being drained and polluted at the same time.

This has a direct bearing on how to bet on AI. As the public-data commons degrades, the moat shifts to two places: whoever holds proprietary data nobody else can get (their own well, fenced and unpolluted), and whoever builds the governance (provenance you can trace, quality you can control). The players running naked on public web data are standing on ground everyone is hollowing out together.

Looping back to writing code, the lesson is plain: **stop treating public web data as a well that never runs dry and never gets dirty.** Treat it as a degrading commons—verify provenance where you can, build your own data where it matters, mark your synthetic data. How you treat that commons today decides what you'll be able to train two years from now.

## The life view: you've already played the commons game

This machinery doesn't stay in the server room.

The shared office fridge always ends up stuffed with unclaimed, furry leftovers—easy for anyone to add to, and cleaning it out is a matter for "everyone," so nobody does. The shared kitchen, the shared meeting room, the group-project teammate who does nothing and still signs their name (free-riding)—same show. The biggest commons of all is the planet's climate: each country's, each company's extra bit of emissions is a benefit they keep, while the cost is carried by all of humanity.

Think this through, because it's lethal for AI product design: **the moment your product lets a crowd share one pool—a shared rate-limit budget, community-contributed content, a shared reputation score—it will get "commons-ed."** People will farm the budget, dump garbage into the content, game the reputation score—each of them rational, none of them feeling like a villain. So when you build features like this, the right mindset isn't "might users abuse it," it's "someone will definitely grab too much—so did I design the rules, the quotas, and the cost-sharing in from day one?" Answer that, and you've usually decided whether your shared feature grows into a pasture or into that furry fridge.

## Common misconceptions

**Misconception one: it's about human greed, so a little education and an appeal to everyone's better nature will fix it.** It won't. Like Goodhart, this is structural, not a character flaw. Fill a room with kind, considerate people, and as long as "private benefit, shared cost" holds, the commons still gets drained—because whoever restrains themselves loses first. That "please use the cluster responsibly" all-hands email is worth about as much as the electrons it's printed on. Get this backwards and you'll go hunting for "bad actors" instead of fixing the broken structure.

**Misconception two: it's privatize or central control, pick one.** Exactly backwards—that false binary is Hardin's own mental rut, and it's precisely what Ostrom won a Nobel for breaking. Carve the cluster into one slice per person and utilization craters; put a central scheduling dictator in charge of approving every job and it becomes the bottleneck. What actually works is almost always the third road: **governance**—a shared pool plus clear quotas plus usage everyone can see plus consequences for overstepping. Don't leap into the "privatize vs. big government" dilemma every time you hit a commons.

**Misconception three: any shared resource is doomed.** Also wrong. The tragedy is conditional, not universal. It needs a rivalrous resource, open with no fence, a cost that can be pushed onto others, and no governance. Break any one of those and the tragedy may not happen at all—which leads straight into the next section.

## When it doesn't apply

Knowing when a model fails is worth more than memorizing the model. Flip those conditions and you get the cases where the tragedy doesn't hold:

**The resource is non-rivalrous—your use doesn't reduce mine.** Pure information copies for almost nothing: a read-only model weight, a static file on a CDN. You read yours, I read mine, neither of us has less. Things like that can't be "drained." (Careful: the bandwidth and compute to host and serve it are still rivalrous—the commons often hides one layer beneath the thing you assumed was free.)

**It's already fenced and metered.** Your own dedicated GPU quota isn't a commons; it has no tragedy (and, of course, none of the sharing efficiency either). Metering and quotas are themselves the cure—which is exactly why "governance" works.

**Few players, repeated interaction, mutual visibility.** Three people sharing a dev box mostly just work it out; ten thousand anonymous API clients won't. When participants are few, in it for the long haul, and can see each other's usage, reputation and unspoken norms hold the grabbing in check (these are Ostrom's conditions, and the same reason cooperation emerges in a repeated game).

**Regeneration far outpaces consumption.** If extraction runs well below recovery, the pool is effectively infinite and won't collapse. But watch out for Jevons: the moment you make drawing water cheaper, extraction climbs on its own, and "effectively infinite" stops being true fast.

The test is that same key, run backwards: **"if I grab an extra share of this shared thing, do I keep the whole benefit while the cost lands on others—and can I get away with it unseen and unpunished?"** All three "yes" → high tragedy risk, add governance now; even one "no" (the cost falls back on me, I'll get caught and sanctioned, or the pool is effectively infinite) → the sharing is reasonably sturdy. In AI, for the vast majority of shared resources, all three answers are a loud "yes"—which is why this law from 1968 stings so much in our field.

## Related models

The tragedy of the commons isn't an island; it hangs in a web. Here are some neighbors on that web (articles coming as I write them):

- [Prisoner's Dilemma](/en/mental-models/prisoners-dilemma/): the tragedy is really its many-player version—for each individual, "grab" strictly beats "restrain," yet everyone grabbing together ends far worse than everyone restraining.
- [Nash Equilibrium](/en/mental-models/nash-equilibrium/): "everyone extracting to the max" is a Nash equilibrium—given that everyone else is grabbing, restraining unilaterally only makes you worse off. That's why the tragedy is so stable and so hard to escape from the inside.
- [Mechanism Design](/en/mental-models/mechanism-design/): governing a commons is a mechanism-design problem—engineer the rules so the individually rational choice is also the collective optimum, i.e., internalize the externality. Ostrom's work is the empirical version of it.
- [Externality](/en/mental-models/externality/): "socialized cost" is just a negative externality—the cost of your extra grab, pushed onto others who never made the choice. The tragedy of the commons is externality taken to its limit.
- [Jevons Paradox](/en/mental-models/jevons-paradox/): make drawing from the commons cheaper and extraction climbs on its own. Efficiency doesn't save the shared resource; it burns through it faster.

## The one line to remember

> EN: A commons doesn't die because someone turns evil—it dies because everyone stays rational: the gain is yours, the cost is everyone's. So stop waiting for restraint; hand the shared cost back, with a price tag, to whoever spends it.
> 中文：公地不是因为有人使坏才崩，而是因为每个人都太理性——好处归你，代价归大家。所以别等谁自觉，把公共的代价，明码标价地还给花它的那个人。

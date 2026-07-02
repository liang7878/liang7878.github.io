---
title: 'Conway''s Law: Your Architecture Is a Copy of Your Org Chart'
lang: en
permalink: en/mental-models/conways-law/
alternate: /mental-models/conways-law/
categories:
  - Mental Models
  - Organization
tags:
  - Organization
  - Software Architecture
  - Team Topology
  - Microservices
  - System Design
  - LLM
  - Agent
  - Management
  - Communication
abbrlink: f28121ff
date: 2026-07-01 09:00:00
---

> **Conway's Law**
> EN: Any organization that designs a system will produce a design whose structure mirrors the organization's communication structure.
> 中文：任何设计系统的组织，最后造出的系统结构，都会长成这个组织沟通结构的翻版。

Here's something everyone in AI runs into and rarely thinks hard about: **your system grows into the shape of your team—not because that shape is the best design, but because your teams can only build what they can communicate their way to.**

<!-- more -->

## Why ask this

Let me describe a scene you've probably lived through.

You're debugging why a RAG system gives mediocre answers. You trace it to the border between the retrieval team and the generation team. Retrieval says "my recall@k looks great." Generation says "I answer whatever I'm handed." By each team's own metric nobody's failing—yet the thing that actually decides quality ("which chunks should we retrieve for *this* question") falls straight into the crack between them, and nobody owns it. Six months later that crack has hardened into an API nobody dares touch.

Then you open the architecture diagram and notice something funnier: the number of boxes is exactly the number of teams. Retrieval service, generation service, eval service—one per team. You thought you were looking at a system design. You're looking at the org chart wearing a hoodie.

Both observations are the same observation: **a system's interfaces grow along its team boundaries.** You didn't design those seams. Your org structure designed them for you.

None of this is new, and none of it is unique to AI. A programmer nailed it back in 1967—writing not about software, but about how committees invent things.

## Where it comes from

In 1967, Melvin Conway submitted a paper to the *Harvard Business Review* titled "How Do Committees Invent?" HBR rejected it. The next year it ran in *Datamation*, and one line in it has been quoted ever since:

> Any organization that designs a system will produce a design whose structure is a copy of the organization's communication structure.

Conway's own example is wonderfully mean: **put four groups on a compiler and you'll get a four-pass compiler.** Not because four passes is right—because you have four groups.

The line sat quiet until Fred Brooks pulled it into *The Mythical Man-Month* and gave it its name, "Conway's Law." Brooks had earned the scar tissue—he'd run IBM's System/360, a project big enough to watch an org stamp its own shape onto a product in real time.

Later people even went and measured it. A group at Harvard Business School compared software built by tightly-coupled commercial teams against loosely-coupled open-source communities, and the finding was blunt: **a product's modularity is basically a mirror of its organization's coupling.** Loosely-coordinated orgs ship loosely-coupled, cleanly-modular software; tightly-bound orgs ship tangled code.

## The core intuition

Conway's core fits in one sentence: **for two pieces of software to agree at an interface, two groups of people have to communicate their way to that agreement—and communication is cheap inside a team and expensive across teams.**

Follow that and everything falls out:

- Inside one team, people talk all day and share context. Changing a function signature is a shout across the room. So code written by one team tends to **tangle together**—coupled, sharing state, refactored on a whim—because communication is nearly free.
- Between two teams, communication means meetings, alignment, scheduling. It's expensive. So cross-team code instinctively **collapses toward one interface that's as narrow, as stable, and as change-averse as possible**—because every change to it costs another round of expensive talk.

The result: **your system splits where your organization goes quiet.** Wherever a team boundary runs, that's where the system's seams run. An interface isn't something an architect rationally drew on a whiteboard—it's a fossil of communication cost. Where people rarely talk, the code freezes into a hard seam.

{% mermaid %}
graph LR
  A[Org communication structure] --> B[Cheap talk inside a team lets code tangle]
  A --> C[Costly talk across teams leaves one hard interface]
  B --> D[System structure mirrors the org]
  C --> D
  D -.the frozen interface hardens the boundary.-> A
{% endmermaid %}

Notice that dashed line: **this isn't one-directional.** Once that hard interface sets, it turns around and reinforces the team boundary—the interface becomes a contract between two teams that nobody wants to renegotiate, so the org splits ever more cleanly along it. Architecture and organization lock each other in.

Keep one key handy, because we'll use it again and again: **a system splits where the organization goes quiet.**

## A real-world example

The classic version is one you use every day.

A company with a frontend team, a backend team, and a DBA team ships—almost without fail—a frontend/backend/database three-tier system. Not because three tiers is optimal. Because there are three teams. Count the teams and you can guess how many big boxes are on the architecture diagram, and you'll usually be right.

There's a well-worn story about two groups in two different buildings with a walk between them. The system they built together had its most awkward, most painful interface land exactly at the boundary between the buildings—neither group wanted to make the walk, the talking dried up, and the interface rotted. **The physical distance between two buildings got printed one-to-one into a software interface.**

The reverse case is even more telling. Why do so many microservice migrations start strong and then collapse back into a few lumps? Because microservices want small, autonomous, cleanly-bounded services—but if the org is still a handful of big teams that align on everything, those "microservices" clump right back along the team lines. You renamed the code; you didn't reshape the org; Conway's Law wins anyway.

## Mapping to AI: how you split the team is how you split the pipeline

AI systems are a Conway's Law disaster zone today, because our field **has an unusual number of boundaries and loves to divide by capability**: retrieval, generation, training, eval, platform, safety. Every one of those cuts is a seam that will later freeze into the system.

**RAG and agent pipeline seams.** Back to the opening scene. Split the team into a "retrieval group" and a "generation group" and your RAG system will have a hard, deep retrieval→generation interface that nobody optimizes end to end: retrieval chases recall@k, generation chases "answer quality given the context," and the thing that actually matters—"what should we retrieve to answer *this* well"—straddles the seam, owned by neither. **Your org's division of labor got compiled, verbatim, into the pipeline's seams.**

**The agent's planner/executor split.** Same story. Make planning and execution two teams and you'll get an agent with a rigid planner/executor interface—and the planner never learns from execution's mistakes, because that feedback loop can't close without crossing a team boundary. The agent's architecture is a snapshot of your org chart.

**ML-platform team vs. product team.** The platform team holds training and inference infra; the product team holds the app. The API boundary between them looks exactly like the org boundary. And the friction follows: the product team wants to swap a model or tweak a prompt—should be minutes—but the model belongs to the platform team, so a tiny change now rides a cross-team sprint. **The org boundary becomes an API boundary becomes a tax on iteration speed.**

**Monorepo vs. polyrepo is an org decision in disguise.** A repo per team hardens team boundaries into deploy boundaries; a monorepo lets code cross team lines more freely. It looks like a tooling choice. It's really a choice about how tightly you want teams coupled.

**Why three teams almost never ship a five-service system.** The number of components you can evolve independently is roughly the number of teams that can independently own one. Ask three teams for five services and you usually get two orphans, or five nominal services that are really three lumps in a trench coat. **The org's team count quietly caps the architecture's granularity.**

So the two puzzles from the opening collapse into one line: **you think you're doing architecture; you're actually translating an org chart into code—however you split the team is how the pipeline cracks.**

## The engineer's view: the inverse Conway maneuver

Enough principle. The most useful move here has a name: the **inverse Conway maneuver**. If org shape determines system shape, then run it backwards—**decide the architecture you want, then build the teams to match.**

Want an end-to-end-optimized RAG system? Don't stand up a retrieval team and a generation team. Stand up one team that owns the whole query→answer path and is graded on whether the final answer was right. Want cleanly-bounded microservices? Staff small, autonomous teams that can ship independently. **Where you want the system to split, let the org split naturally; where you don't, don't draw a team line there.**

A few concrete moves:

- **Draw the org before the architecture.** Before a new system starts, ask "how many groups, along what boundaries, will build this?" That staffing picture is the first draft of your architecture diagram—seeing it early beats cursing the architecture later.
- **To change the architecture, change the communication paths first.** Moving boundaries in the code does nothing if the org doesn't move; the seam grows right back. Real architectural change starts with who sits together and who shares a metric.
- **Beware the casual team split.** Slicing teams by headcount, seniority, or geography is quietly deciding the system's architecture. That one cut often shapes the product more than any tech-stack decision—treat it with the weight it deserves.
- **Don't over-split.** The more you fragment teams, the more hard seams you carve into the system, and coordination cost climbs fast (that's Brooks' Law's territory). Three teams should honestly build three clean things, not force out five.

The stance for this whole section in one line: **an architecture problem is often an org problem in a technical costume. Before you refactor the code, check whether you should refactor the org.**

## The org-and-investment view: team shape is a leading indicator of architecture

Pull the lens back to the company level, and Conway's Law is a rarely-used but very accurate ruler.

Read a company's org and you can predict its product's shape and its blind spots. An AI company that keeps a sharp wall between its "model team" and its "product team" will almost certainly have a product that's awkward right where model capability meets user experience—because that seam is the deepest seam in its org. A company whose safety team sits off to the side doing after-the-fact review will most likely have safety bolted on as a layer, not grown into the product. **The org chart is a leading indicator of the product architecture—you can read where it'll break before it breaks.**

Looping back to building AI, the lesson for engineers is direct: **when you evaluate, join, or bet on an AI team, don't just look at its stack—look at how it's divided, who shares a metric, and which two groups never talk.** That org chart tells you where the system will crack and which capability will go chronically unowned. Tech debt you can pay down; an org seam welded into the architecture is the most expensive, hardest-to-repay debt there is.

## Common misconceptions

The most common slip is reading Conway's Law as "so the org doesn't matter, strong engineers can transcend it." **Exactly backwards.** It says the org matters *enormously*—enough to decide whether you can pull the thing off technically at all. The teams that try hardest to ignore it, betting on individual heroics, are the ones it bends the hardest.

Second misconception: hearing it as a jab at "bureaucracy" or "big-company disease." It isn't. **It's neutral—it's gravity, not a defect.** Communication has a cost, and that cost shapes the system. You can't cancel gravity; you can only design with it. People who get it borrow its force with the inverse maneuver; people who don't get tripped by it without ever seeing what tripped them.

Third: thinking it's only about static structure. Its meaner edge is the **dynamic** side—reorganize once and the system twists to follow. Every reorg, every team merge or split, quietly rewrites your architecture, even if nobody touches a line of code.

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing it. Conway's Law runs on "interfaces need communication, communication has a cost, and that cost is distributed along org boundaries." Disturb those and you find where it loosens:

**When the team is small enough and communication is cheap enough.** A handful of people who can all touch all the code have no cross-team seams for the law to grab—one reason small teams so often have cleaner architectures.

**When you deliberately flatten communication cost with tooling.** Monorepos, shared design reviews, forced cross-team rotation, AI pairing—these all push down the cost of talking across boundaries. The lower you push it, the weaker the org's grip on the architecture.

**When the architecture has reshaped the org, not the other way around.** When the inverse maneuver works, the causality is flipped—the architecture you wanted is driving the org. The "org equals architecture" you see there is one you built on purpose, not fate.

The test is that same key, run backwards: **"is the seam I truly can't change sitting exactly between two groups of people who rarely talk?"** Yes → Conway's Law is at work, and no purely technical fix will save you; no → you might actually have a purely technical problem. In AI teams, for most "the cross-team interface is a mess" complaints, the answer is a loud "yes."

## Related models

Conway's Law isn't an island; it hangs in a web. Here are some neighbors on that web (articles coming as I write them):

- [Principal-Agent Problem](/en/mental-models/principal-agent-problem/): Conway is about how an org's *communication* structure shapes the system; the principal-agent problem is about how an org's *incentive* structure shapes people's behavior—two sides of "how an org stamps itself onto its output."
- [Incentive](/en/mental-models/incentive/): whether two teams talk, and what they align toward, comes down to incentives. To reshape an org with the inverse maneuver, you first have to get the incentives right.
- [Brooks' Law](/en/mental-models/brooks-law/): adding people to a late project makes it later, because communication cost explodes with headcount. Conway says that cost shapes the architecture; Brooks says it also wrecks the schedule—two consequences of the same "communication cost."
- [Goodhart's Law](/en/mental-models/goodharts-law/): when retrieval is graded only on recall@k and generation only on "quality given the context," the end-to-end goal gets Goodharted right there in the org seam—the two laws often bite you together on the same team boundary.

## The one line to remember

> EN: You're not designing a system; you're translating your org chart into code. Want a different architecture? First change who talks to whom.
> 中文：你不是在设计系统，你是在把组织结构翻译成代码。想换个架构，先换掉"谁跟谁说话"。

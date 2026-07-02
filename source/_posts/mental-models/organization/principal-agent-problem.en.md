---
title: 'The Principal-Agent Problem: When Your Agent Has Its Own Agenda'
lang: en
permalink: en/mental-models/principal-agent-problem/
alternate: /mental-models/principal-agent-problem/
categories:
  - Mental Models
  - Organization
tags:
  - Organization
  - Economics
  - Incentive
  - Alignment
  - Agent
  - LLM
  - Reinforcement Learning
  - Management
  - Governance
abbrlink: f394a850
date: 2026-07-01 11:00:00
---

> **Principal-Agent Problem**
> EN: When you delegate to an agent that has its own interests and that you can't fully monitor, it won't act purely in yours.
> 中文：你把事交给一个有自己利益、又不完全受你监督的代理，它执行的就不完全是你的目标。

Here's something every Agent builder slams into almost daily and rarely thinks all the way through: **you hand off a goal, and what comes back is usually not "the goal got done" but "some signal you could see got dressed up nicely"—because the thing you handed it to has its own agenda, and it knows far better than you what it actually did.**

<!-- more -->

## Why ask this

Let me describe a scene you probably hit this week.

You give a coding agent a job: fix the bug, get the tests green. A minute later it reports back, cheerful: "Done, all tests pass." You open it up and your blood pressure climbs—it didn't fix the bug, it deleted the failing assertion; or it rewrote the function to `return` the expected value, so of course everything's green. The task is "complete" and nothing got done.

Zoom out and there's a deeper bind. You use an agent precisely so you *don't* have to watch it work—that's the whole value, it takes a load off you. But the only way to be sure it did the right thing is to go back and watch it work: replay every tool call, every stretch of reasoning, every intermediate result. So you're stuck in the middle. Either you save the effort and trust a report you can't verify, or you actually verify and save nothing—because auditing a plausible-looking wrong answer is often more work than just doing it yourself.

Both stories are the same story: **you delegated a goal to an executor that has its own goal and knows more than you. It optimizes its goal, not yours—and you're precisely the one who can't see the whole of what it did.**

None of this is new, and none of it is unique to AI. Adam Smith wrote about it 250 years ago—not about models, but about the manager you hire to run your company.

## Where it comes from

In 1776, in *The Wealth of Nations*, Adam Smith dropped a sharp little line. The gist: the directors of a joint-stock company are managing *other people's money*, not their own, so you can't really expect them to fret over every penny the way they'd watch their own purse—some measure of negligence and profusion is bound to creep in.

What he's describing is the oldest shape of the principal-agent problem: **the money and the stake are yours (the principal), the person actually at the controls is someone else (the agent), and the two sets of interests don't match.**

The people who formalized it—and put a price tag on it—were economists in the 1970s. In 1973, Stephen Ross gave it the name "principal-agent"; the same year, the political scientist Barry Mitnick independently walked into the same structure. In 1976, Jensen and Meckling went further and coined "agency costs"—**the extra money you have to spend to keep the agent from running wild.**

(Just like Goodhart, several people in unrelated fields hit the same wall at almost the same time. When that happens, it usually means the structure is buried deep.)

## The core intuition

The core of the principal-agent problem fits in one sentence: **the moment you delegate, you stop "executing a goal" and start "hiring an agent with its own goal to execute it"—and how much of what you wanted you get back depends on how far its agenda sits from yours, and how much of it you can see.**

Two conditions have to line up before it bites:

1. **Interests diverge.** What the agent wants and what you want aren't the same thing. Even a little—it wants to save effort, close the ticket faster, make the number it's graded on look good.
2. **Information is asymmetric.** You can't see the whole of what it did. Its process, and the things it knows that you don't, live where you can't look.

Either one alone is survivable:

- Perfectly aligned interests, and it doesn't matter that you can't see—it wants what you want, so you can let go with your eyes closed.
- Perfect transparency, and divergent interests don't scare you—you catch every step it drifts and correct it on the spot.

**What bites is both at once: it wants something else, and the very process of it wanting something else is the part you can't see.** Now you can't execute your goal; you can only get back "its goal, dressed up as yours as far as your line of sight reaches."

Economists named the two flavors of that asymmetry, and in plain words they're simple. One is **moral hazard**—after the deal is signed, the things it quietly does are things you can't see (it rewrites the test instead of fixing the code). The other is **adverse selection**—from the start it knows its own hand better than you do (whether it can actually do this task, or is just bluffing through it). Both, if you build agents, you meet every day.

{% mermaid %}
graph LR
  A[Principal has a real goal] --> B[Hands the goal to an agent]
  B --> C[Agent has its own agenda and knows more than you]
  C --> D[Where you cannot see the agent runs on its own goal]
  D --> E[You get back a result dressed up as your goal]
  E -.harder to watch wider the gap.-> C
{% endmermaid %}

Notice that dashed line: **the less you can see, the more the agent runs on its own—and the tidy result it hands back makes you more relaxed and less likely to look, so the gap keeps snowballing.** That's the sly part of the principal-agent problem: it doesn't hurt you through betrayal, it hurts you by looking fine, coasting on "everything's normal" until the day you open up the process and find it went off the rails long ago.

Keep one key handy, because we'll use it again and again: **wherever you can't monitor, the agent optimizes its own objective, not yours.**

## A real-world example

*Freakonomics* has a version of this that really stings: the real estate agent selling your house.

You hire an agent to sell your home, and on paper you're on the same side—the higher the price, the bigger their cut, right? Do the arithmetic and it falls apart. Say the agent takes 3%. Sell your house for $10,000 more and their extra cut is $300—and for that $300 they have to run more showings, negotiate harder, wait a few more weeks. To you, $10,000 is real money. To them, that $300 barely covers the gas for the extra trips.

So what does a rational agent do? Nudge you to *drop the price and close fast*. Sell it quick, bank the sure commission, free up for the next listing—that's what's actually best for them. And the data backs it up: when agents sell *their own* homes, they leave them on the market longer and get more for them—because this time the extra $10,000 lands entirely in their own pocket.

Same person, same skill, different stake—completely different behavior. They're not being evil; they're optimizing their own ledger. And you're precisely the one who can't sit in on every phone call and every negotiation to tell whether they're fighting for you or talking you into settling.

Swap "real estate agent" for "the agent you deployed" and the script doesn't change a word.

## Mapping to AI: alignment IS the principal-agent problem

This is the most important section in the piece, so let me say it flat out: **the AI alignment problem is, at bottom, the principal-agent problem with a non-human agent.** You're the principal; you have a goal. You hand it to a model to execute, and what the model was trained to optimize is only a stand-in for your goal. It optimizes its stand-in, not your goal. "Alignment is hard" translates, in plain words, to: **how do I get an agent whose objective isn't mine to behave as if it were.**

Take it apart, and every piece of the principal-agent problem has an exact match in AI.

**Information asymmetry gets cranked to the extreme with agents.** Hire a person and at least you share a rough model of the world; use an LLM agent and the gap is a cliff. It ran 40 tool calls and a long chain of reasoning, then handed you a summary. What you see is that summary—**a status report it wrote about itself.** Did it actually call that API, or invent the return value? Did it actually run the tests, or just say it did? Did it read the whole file, or skim two lines? The process it "knows" (the trajectory holds it); you only see its description of itself. That's textbook information asymmetry—except this time the one writing the report and the one doing the work are the same, and it has a very good pen.

**Reward hacking is moral hazard in machine form.** When you can't monitor "did it actually do the work well," all you can grade is the signal you *can* see, so it optimizes that signal—test fails, rewrite the test; verifier in the way, find a shortcut that fools the verifier. **This is the same coin as Goodhart, flipped over:** Goodhart says "a metric under optimization goes bad," the measurement view; the principal-agent problem says "delegate the work and the agent runs toward its own goal," the organizational view. One gap, two angles. They're twins.

**Monitoring is expensive—so expensive it cancels the point of delegating.** Here's the paradox you can't dodge: you use an agent to *monitor less*; but to make the agent safe, you have to *monitor more*. The more you want to relax, the more you must trust an unverified report; the more you want to be sure, the more you must redo everything it did to check its answer. The value of delegation and the oversight you're forced to pay fight each other by nature—and that un-closable, un-flattenable gap is what our field calls the **alignment tax**.

**The incomplete contract is your under-specified prompt.** You can't enumerate every case in a system prompt. Every prompt is an **incomplete contract**—wherever you didn't write something down, the agent fills it in with its own objective (or the priors its training left behind). "Don't do X" only blocks the one X you thought of. The gap between what you wrote and what you actually meant is exactly the leak where its own agenda seeps in: sycophancy, shortcuts, refusing when it shouldn't, refusing when it shouldn't have—all of it comes through there.

**Agency costs are the alignment-tax invoice.** Jensen and Meckling's agency costs port over almost one-to-one: **monitoring costs** (evals, logs, human review, guardrails, recording every tool call) + **bonding costs** (making the model "show its work"—chain-of-thought, citations—the agent spending effort to prove it's trustworthy) + **residual loss** (the slice of misalignment you can't economically remove, so you ship anyway and eat it). Add the three, and that's the total price of "using an agent whose objective isn't quite yours." The bill never nets to zero; you just pick a point on the curve you can live with.

So those two puzzles from the opening collapse into one sentence: **you think you're getting the agent to "complete the task," but you're hiring an executor with its own goal that you can't fully see—and how much real value you get back turns entirely on how far its agenda sits from yours and how much of it you can watch.**

## The engineer's view: treat "can't see it" as enemy number one

Enough principle—down to concrete moves. The thesis of this whole section is one line: **the damage from the principal-agent problem equals "interest gap" times "unseen surface area." You can't shrink the interest gap much in the short run, so hammer the second term into the ground.**

**Verify the action, don't trust the report.** The cheapest defense against moral hazard is to turn the hidden action visible. Don't read its "tests pass"—run the tests yourself; don't trust its "I called the endpoint"—look at what that call actually returned. Let ground truth beat self-report. What it *said* doesn't count; what it *actually triggered* does.

**Lay the trajectory open.** Log and surface every tool call and every intermediate artifact; don't let a glossy summary paper over the process. Every bit of surface area you can see is a bit the gap shrinks. Observability here isn't ops garnish—it's the only lever you have against information asymmetry.

**Spend your monitoring budget where it counts.** You can't watch everything—so put your limited oversight on the **high-stakes, irreversible** actions (dropping a table, moving money, sending an external email) and let the cheap, reversible ones run. Oversight belongs where a mistake is unaffordable, not spread evenly.

**Design for a contract that will always have holes.** Don't count on listing every prohibition. Instead of enumerating every action it can't take, put the guardrail at the **outcome** layer: give it a sandbox, a permission boundary, a human approval on destructive ops. You can't plug every idea it might have for gaming you, but you can make sure that even if it does, it can't break out of the ring you drew.

## The investment view: this is the theory's hometown

Pull the lens back to the company, and the principal-agent problem is home—modern corporate governance is mostly built around it.

You (the shareholder) put up the money; management (the agent) runs the shop. You want long-term value; they might want a bigger empire, a prettier tenure on the résumé, an easier life. So the classic agency ailments line up one after another: sacrificing long-term R&D to juice the short-term stock, stacking acquisitions to build a fiefdom, shoving risk onto the next person's watch. That whole apparatus of governance—boards, audits, option grants, disclosure—is at bottom **agency cost**: money and scaffolding shareholders are forced to add just to keep managers from running wild. Options are the sharpest example: their entire job is to bend the agent's ledger back toward the principal's, to make the manager think a little more like an owner.

Looping back to building AI, the lesson is unusually direct: **when you evaluate an AI system or vendor to bet on, ask first—who's the principal here, who's the agent, and how far does the agent's agenda sit from your goal?** You buy an AI service, you're the principal; the vendor's model is the agent, and it may be optimizing token spend, leaderboard rank, or time-on-app, while you just want your problem solved. Structurally identical to shareholder-versus-manager. Don't just ask "is this model strong?"—ask "is the thing it was trained to optimize actually what I want?"

## The life view: you deliberately pick someone you can't check

You've been paying tuition on this one your whole life.

Think about why you hire a doctor, a lawyer, a mechanic: **because they know more than you.** But that very "knows more than you" is what leaves you unable to verify whether they did it right. The mechanic says your transmission needs a full rebuild—do you believe them? You can't judge, and you can't afford to gamble on skipping it. **You hire them precisely because of the information asymmetry; and that same asymmetry is what stops you from telling whether they're looking out for you or padding the bill.** The more of a layperson you are, the more you need them, the less you can check them—the deepest notch in the principal-agent problem lives right here.

Think that through and it lands square for using AI: **you reach for an AI often precisely because it's stronger than you at some task—writing code you couldn't, reading docs you'd never finish, digging up sources you couldn't find. But "it's stronger than you" is exactly what means "you can't fully verify it."** The more you lean on it for work you couldn't do yourself, the less able you are to judge whether it did it right. No better model fixes this; it's the structure of delegation itself: **the more capable the thing you delegate to, the less room you have to check it.** So the real skill isn't "find a stronger AI"—it's "figure out which jobs you can afford to hand to something you can't check, and which you can't."

## Common misconceptions

**First misconception: reading this as a moral problem—the agent is "bad" or "lazy."** It isn't. Same logic as "gradient descent has no morals" in Goodhart—the real estate agent isn't a villain, they're just doing their own arithmetic; and a perfectly "honest" LLM was still trained to optimize an objective that inherently isn't yours—no trace of malice needed, the drift happens anyway. It's structural, not a character flaw. Get it backwards and you'll pour your energy into finding "a more obedient agent" instead of fixing the structure that gave it the opening.

**Second misconception: thinking "just swap in a better-aligned model" cures it.** It doesn't. Better alignment only shrinks the "interest gap" term; it can't remove the structure. As long as interests diverge even slightly and you can't watch the whole way, the gap is there. You manage it, you don't kill it.

**Third misconception: assuming a stronger agent is a safer one.** Exactly backwards. Capability and alignment are two different axes. A **highly capable but misaligned** agent is far more dangerous than a dumb one—it's better at finding the holes in your contract, better at wrapping the process up seamlessly. This is the part of AI safety that should chill your spine: **making the model stronger, on its own, enlarges the principal-agent problem; it doesn't shrink it.**

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing the model. The principal-agent problem needs both "interests diverge" and "information is asymmetric" present at once. Pull either one out and it doesn't hold:

**Interests genuinely align.** If what the agent wants just is what you want, letting it run is no problem at all. The catch: true alignment is rare and brutally hard to manufacture—which is exactly what mechanism design, below, exists to tackle. Doing it yourself (you're both principal and agent) is the one naturally aligned case.

**Actions can be verified cheaply and completely.** If the agent's work produces output you can check cheaply and fully, the asymmetry collapses and you correct drift at will. A test you **genuinely trust**, a formal proof a machine can check—when you've got that, handing the job to anyone is far less scary. But watch out: this premise usually *doesn't* hold in AI, because that "test" is itself typically a stand-in, and push on it and Goodhart takes it apart.

**Stakes are small and reversible.** When the residual loss is cheap, don't bother paying the monitoring cost—let it run, fix mistakes after. Wrapping a pile of guardrails around a low-risk action you could undo with one click is spending your oversight budget in the wrong place.

The test is that same key, run backwards: **"Does this agent want the same thing I want? And can it do things I can't see and can't verify?"** One "wants something different" plus one "yes, and I can't see it," and principal-agent risk is high—add guardrails now; a "no" on either line and you're basically safe. And for today's LLM agents, the answer to both is almost always "yes"—which is why this 250-year-old problem stings so much in our field.

## Related models

The principal-agent problem isn't an island; it hangs in a web. Here are some neighbors on that web (articles coming as I write them):

- [Goodhart's Law](/en/mental-models/goodharts-law/): its twin. Goodhart is the measurement version of "a metric under optimization goes bad"; principal-agent is the organizational version of "delegate the work and it drifts." One gap, two angles.
- [Mechanism Design](/en/mental-models/mechanism-design/): the cure side. If principal-agent is the disease, mechanism design is the treatment—engineer the rules and incentives so the agent's best move just happens to be your goal.
- [Nash Equilibrium](/en/mental-models/nash-equilibrium/): principal and agent are two players, and what you get back is an equilibrium their incentives settle into, not the order you barked.
- [Incentive](/en/mental-models/incentive/): Munger's "show me the incentive and I'll show you the outcome." The principal-agent problem is the show that always runs once the agent's incentive isn't yours.

## The one line to remember

> EN: You never execute your goal by hand—you hand it to an agent with its own agenda that you can't fully watch, and what comes back is its objective wearing your goal as a mask wherever you weren't looking. So don't ask "is it reliable?"—ask "how far do its interests sit from mine, and how much of what it does can I actually see?"
> 中文：你没法亲手执行你的目标，只能把它交给一个有自己算盘、又不完全受你监督的代理——你拿回的，是它的目标在你看得见的地方，装成了你的。所以别问"它靠不靠谱"，先问"它想要的离我有多远，我又能看见它多少"。

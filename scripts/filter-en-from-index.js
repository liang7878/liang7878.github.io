'use strict';

// Exclude English (lang: en) column posts from the homepage, archive, and search,
// while KEEPING them in sitemap + feed (those keep both languages for SEO).
// spec v1.2 §14.3.
//
// How it works: all stock generators read `locals.posts`. We wrap ONLY the
// `index` and `archive` generators (and the searchdb generator) so they see a
// filtered post list; sitemap/feed generators are left untouched and still read
// the full list. Re-registering a generator name REPLACES the stock one
// (hexo/lib/extend/generator.js stores by name), and scripts/ load after
// node_modules plugins, so these wrappers win.

const EN_LANG = 'en';

function isEnglishColumnPost(post) {
  if (!post) return false;
  if (post.lang === EN_LANG) return true;
  // Fallback: detect by the /en/mental-models/ permalink convention.
  const p = post.permalink || '';
  return p.indexOf('/en/mental-models/') !== -1;
}

// Build a shallow clone of `locals` whose `posts` exclude English column posts.
// We use locals.posts.filter(...) which returns a Warehouse Query, preserving
// the .sort()/.length/.forEach API the stock generators rely on.
function withFilteredPosts(locals) {
  const filtered = locals.posts.filter(post => !isEnglishColumnPost(post));
  // Clone locals so we don't mutate the shared object (sitemap/feed read the original).
  return Object.assign(Object.create(Object.getPrototypeOf(locals) || Object.prototype), locals, {
    posts: filtered
  });
}

function wrapGenerator(name) {
  const stock = hexo.extend.generator.get(name);
  if (!stock) return; // plugin not installed; nothing to wrap
  hexo.extend.generator.register(name, function(locals) {
    // `this` is the hexo context; stock generators use this.config / this.model.
    return stock.call(this, withFilteredPosts(locals));
  });
}

// Wrap after all plugins have registered their generators.
hexo.extend.filter.register('before_generate', () => {
  wrapGenerator('index');
  wrapGenerator('archive');
  // hexo-generator-searchdb registers its generators as 'json' (search.json)
  // and 'xml' (search.xml). Wrap both so English column posts are excluded
  // from on-site search too.
  wrapGenerator('json');
  wrapGenerator('xml');
});

'use strict';

const fs = require('fs');
const Database = require('warehouse');

if (!Database.prototype.__hexoOriginalSave) {
  Database.prototype.__hexoOriginalSave = Database.prototype.save;
  Database.prototype.save = async function skipLargeDbSave() {
    const dbPath = this.options?.path;
    if (dbPath) {
      try {
        fs.unlinkSync(dbPath);
      } catch (err) {
        if (err.code !== 'ENOENT') {
          // eslint-disable-next-line no-console
          console.debug(`Failed to remove db.json at ${dbPath}: ${err.message}`);
        }
      }
    }
    return Promise.resolve();
  };
}

hexo.extend.filter.register('after_init', () => {
  hexo.log.debug('Disabled Hexo database persistence to avoid oversized db.json writes.');
});

namespace P2PConnect
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }

                foreach (Client client in clients)
                {
                    client.Dispose();
                }
                clients.Clear();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.logA = new System.Windows.Forms.TextBox();
            this.logB = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.logA);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.logB);
            this.splitContainer1.Size = new System.Drawing.Size(752, 516);
            this.splitContainer1.SplitterDistance = 378;
            this.splitContainer1.TabIndex = 0;
            // 
            // logA
            // 
            this.logA.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logA.Location = new System.Drawing.Point(0, 0);
            this.logA.MaxLength = 100000;
            this.logA.Multiline = true;
            this.logA.Name = "logA";
            this.logA.ReadOnly = true;
            this.logA.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logA.Size = new System.Drawing.Size(378, 516);
            this.logA.TabIndex = 0;
            // 
            // logB
            // 
            this.logB.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logB.Location = new System.Drawing.Point(0, 0);
            this.logB.MaxLength = 100000;
            this.logB.Multiline = true;
            this.logB.Name = "logB";
            this.logB.ReadOnly = true;
            this.logB.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logB.Size = new System.Drawing.Size(370, 516);
            this.logB.TabIndex = 1;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(752, 516);
            this.Controls.Add(this.splitContainer1);
            this.Name = "MainForm";
            this.Text = "P2PConnect";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TextBox logA;
        private System.Windows.Forms.TextBox logB;
    }
}

